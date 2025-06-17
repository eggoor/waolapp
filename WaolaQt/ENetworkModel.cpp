#include "ENetworkModel.h"

using namespace Wpp;

ENetworkModel::ENetworkModel(QObject* parent, NetworkPtr network, PersistorPtr persistor)
	: QAbstractTableModel{parent}
	, network(network ? std::move(network) : Wpp::INetwork::create(NetworkParams {
		nullptr,
		&IScanner::create,
		&IVault::create,
		nullptr,
		[this](auto&& arg1) { onHostDetected(std::forward<decltype(arg1)>(arg1)); },
		[this](auto&& arg1, auto&& arg2) {
			onHostChanged(std::forward<decltype(arg1)>(arg1),
				std::forward<decltype(arg2)>(arg2));
		},
		[this](auto&& arg1) { onIsIdleChanged(std::forward<decltype(arg1)>(arg1)); },
	}))
	, persistor(persistor ? std::move(persistor) : IPersistor::create())
{
	this->network->init(this->persistor->restore());
}

int ENetworkModel::rowCount(const QModelIndex& x) const
{
	return x.isValid() ? 0 : static_cast<int>(network->hostCount());
}

int ENetworkModel::columnCount(const QModelIndex& x) const
{
	return x.isValid() ? 0 : arrsiz(columnNames);
}

QVariant ENetworkModel::data(const QModelIndex& x, const int role) const
{
	auto [xHost, field] = getValidCell(x);
	const auto& host = network->at(xHost);

	switch (field) {
		case HostField::DisplayName:
		{
			return displayName(host, role);
		}
		case HostField::Hostname:
		{
			return hostname(host);
		}
		case HostField::IpAddr:
		{
			return ipAddress(host);
		}
		case HostField::MacAddr:
		{
			return macAddress(host);
		}
		case HostField::LastSeenOnline:
		{
			return lastSeenOnline(host);
		}
		case HostField::OpResult:
		{
			return opResult(host);
		}
		case HostField::None:
		default:
		{
			throw std::out_of_range("Requested host field #" + std::to_string(static_cast<int>(field))
				+ " exceeds max host field number (" + std::to_string(static_cast<int>(HostField::OpResult)) + ")");
		}
	}

	return {};
}

bool ENetworkModel::setData(const QModelIndex& x, const QVariant& value, const int role)
{
	bool success(false);
	auto [xHost, field] = getValidCell(x);
	const auto& host = network->at(xHost);
	auto sValue = value.toString().toStdString();

	switch (field) {
	case HostField::DisplayName:
	{
		if (host->setDisplayName(std::move(sValue))) {
			emit dataChanged(x, x);
			success = true;
		}
		break;
	}
	case HostField::Hostname:
	{
		if (host->setHostname(std::move(sValue))) {
			emit dataChanged(x, x);
			success = true;
		}
		break;
	}
	case HostField::IpAddr:
	{
		if (host->setIpAddress(sValue)) {
			emit dataChanged(x, x);
			success = true;
		}
		break;
	}
	case HostField::MacAddr:
	{
		if (host->setMacAddress(sValue)) {
			emit dataChanged(x, x);
			success = true;
		}
		break;
	}
	case HostField::LastSeenOnline:
	case HostField::OpResult:
	case HostField::None:
	default:
	{
		throw std::out_of_range("Requested host field #" + std::to_string(static_cast<int>(field))
			+ " exceeds max host field number (" + std::to_string(static_cast<int>(HostField::OpResult)) + ")");
	}
	}

	return success;
}

QHash<int, QByteArray> ENetworkModel::roleNames() const
{
	static QHash<int, QByteArray> roles = {
		{Qt::DisplayRole, "display"}
		, {LastSeenOnlineRole, "lastSeenOnline"}
		, {StoredRole, "stored"}
	};

	return roles;
}

void ENetworkModel::onHostDetected(const int xHost)
{
	QMetaObject::invokeMethod(this, [this, xHost]() {
		beginInsertRows(QModelIndex(), xHost, xHost);
		endInsertRows();
	}, Qt::QueuedConnection);
}

void ENetworkModel::onHostChanged(const int xHost, const HostField field)
{
	emitCellChanged(xHost, field);
}

void ENetworkModel::onIsIdleChanged(const bool isIdle)
{
	QMetaObject::invokeMethod(this, [this, isIdle]() {
		emit isIdleChanged(isIdle);
	}, Qt::QueuedConnection);
}

void ENetworkModel::emitCellChanged(const int xRow, HostField field)
{
	QMetaObject::invokeMethod(this, [this, xRow, field]() {
		const auto roles = field != HostField::LastSeenOnline
			? QList<int>{ Qt::DisplayRole }
			: QList<int>{ LastSeenOnlineRole };

		emit dataChanged(index(xRow, static_cast<int>(field)),
			index(xRow, static_cast<int>(field)), roles);
	}, Qt::QueuedConnection);
}

std::tuple<int, HostField> ENetworkModel::getValidCell(const QModelIndex& x) const
{
	const auto xHost = x.row();

	if (network->hostCount() < x.row())
		throw std::out_of_range("Requested host #" + std::to_string(xHost)
			+ " exceeds host count (" + std::to_string(network->hostCount()) + ")");

	const auto field = x.column();

	if (field > static_cast<int>(HostField::OpResult))
		throw std::out_of_range("Requested host field #" + std::to_string(field)
			+ " exceeds max host field number (" + std::to_string(static_cast<int>(HostField::OpResult)) + ")");

	return {xHost, static_cast<HostField>(field)};
}

QVariant ENetworkModel::headerData(const int section, const Qt::Orientation orientation, const int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		return section < arrsiz(columnNames) ? columnNames[section] : "Undefined";
	}

	return {};
}

Qt::ItemFlags ENetworkModel::flags(const QModelIndex& x) const
{
	return x.isValid()
		? Qt::ItemIsSelectable | Qt::ItemIsEnabled
		: Qt::NoItemFlags;
}

bool ENetworkModel::isEmpty() const
{
	return network->isEmpty();
}

bool ENetworkModel::isIdle() const
{
	return network->isIdle();
}

bool ENetworkModel::addHost(const QString& displayName, const QString& hostname,
	const QString& ipAddress, const QString& macAddress)
{
	const int xHost = static_cast<int>(network->hostCount());
	beginInsertRows(QModelIndex(), xHost, xHost);
	const bool added = network->addHost(displayName.toStdString(), hostname.toStdString(),
		ipAddress.toStdString(), macAddress.toStdString());
	endInsertRows();

	return added;
}

bool ENetworkModel::removeHosts(const QList<QModelIndex>& xx)
{
	if (xx.isEmpty()) {
		return false;
	}

	std::vector<size_t> fatedHosts;
	fatedHosts.reserve(xx.size());

	for (const QModelIndex& x : xx) {
		if (!x.isValid() || x.parent().isValid() || x.row() < 0
			|| x.row() >= network->hostCount()) {
			return false;
		}
		fatedHosts.emplace_back(x.row());
	}

	std::sort(fatedHosts.begin(), fatedHosts.end(), std::greater<>());

	auto xHost = fatedHosts[0];
	int count = 1;
	for (size_t i = 1; i <= fatedHosts.size(); ++i) {
		if (i == fatedHosts.size() || fatedHosts[i] != fatedHosts[i - 1] - 1) {
			xHost -= count - 1;
			beginRemoveRows(QModelIndex(), static_cast<int>(xHost),
				static_cast<int>(xHost + count - 1));

			try {
				network->removeHosts(xHost, count);
			} catch (const std::exception& ex) {
				endRemoveRows();
				qmlEngine(this)->throwError(tr(ex.what()));
			}

			endRemoveRows();

			if (i < fatedHosts.size()) {
				xHost = fatedHosts[i];
				count = 1;
			}
		} else {
			++count;
		}
	}

	return true;
}

void ENetworkModel::wait() const
{
	network->wait();
}

void ENetworkModel::persist()
{
	safeCall([this] {
		persistor->persist(network->getHosts());
	});
}

QVariant ENetworkModel::displayName(const HostPtr& host, const int role)
{
	switch (role) {
		case Qt::DisplayRole: {
			return QString::fromStdString(host->getDisplayNameSmart());
		}
		case StoredRole: {
			return QString::fromStdString(host->DisplayName);
		}
		default: {
			return {};
		}
	}

	return {};
}

QVariant ENetworkModel::hostname(const HostPtr& host)
{
	return QString::fromStdString(host->Hostname);
}

QVariant ENetworkModel::ipAddress(const HostPtr& host)
{
	return QString::fromStdString(host->IpAddress.toString());
}

QVariant ENetworkModel::macAddress(const HostPtr& host)
{
	return QString::fromStdString(host->MacAddress.toString());
}

QVariant ENetworkModel::lastSeenOnline(const HostPtr& host)
{
	return QVariant::fromValue(QDateTime::fromSecsSinceEpoch(host->LastSeenOnline));
}

QVariant ENetworkModel::opResult(const HostPtr& host)
{
	switch (host->OpResult) {
	case Wpp::OpResult::orUndefined:
		return QString();
	case Wpp::OpResult::orFail:
		return QString("Fail");
	case Wpp::OpResult::orSuccess:
		return QString("Success");
	default:
		return QString();
	}
}

void ENetworkModel::wakeUp(const QList<QModelIndex>& xx)
{
	std::vector<int> hosts;
	hosts.reserve(xx.size());

	std::transform(xx.begin(), xx.end(), std::back_inserter(hosts),
		[](const QModelIndex& x) { return x.row(); });

	safeCall([this](std::vector<int>&& targets) {
		network->wakeUp(std::move(targets));
	}, std::move(hosts));
}

void ENetworkModel::scanNetwork()
{
	safeCall([this] {
		network->scanAsync();
	});
}

void ENetworkModel::refresh()
{
	safeCall([this] {
		network->refreshAsync();
	});
}

void ENetworkModel::finalizeScan()
{
	safeCall([this] {
		network->finalizeAsyncOp(true);
	});
}

void ENetworkModel::cancel() const
{
	network->cancel();
}
