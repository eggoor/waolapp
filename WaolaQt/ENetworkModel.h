#ifndef NETWORK_MODEL_H
#define NETWORK_MODEL_H

#include <QQmlEngine>
#include <QAbstractTableModel>

#include <INetwork.h>

#include "IPersistor.h"

// ReSharper disable once CppClassCanBeFinal
class ENetworkModel : public QAbstractTableModel {
	Q_OBJECT
	QML_ELEMENT
	Q_DISABLE_COPY(ENetworkModel)

	Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)
	Q_PROPERTY(bool isIdle READ isIdle NOTIFY isIdleChanged)

public:
	explicit ENetworkModel(QObject* parent = nullptr, Wpp::NetworkPtr network = nullptr,
		PersistorPtr persistor = nullptr);

	[[nodiscard]] int rowCount(const QModelIndex& x) const override;

	[[nodiscard]] int columnCount(const QModelIndex& x) const override;

	[[nodiscard]] QVariant data(const QModelIndex& x, int role) const override;

	bool setData(const QModelIndex& x, const QVariant& value, int role) override;

	[[nodiscard]] QHash<int, QByteArray> roleNames() const override;

	[[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	[[nodiscard]] Qt::ItemFlags flags(const QModelIndex& x) const override;

	[[nodiscard]] bool isEmpty() const;

	[[nodiscard]] bool isIdle() const;

public slots:
	void wakeUp(const QList<QModelIndex>& xx);

	void scanNetwork();

	void refresh();

	void finalizeScan();

	void cancel() const;

	void wait() const;

	void persist();

	bool addHost(const QString& displayName, const QString& hostname,
		const QString& ipAddress, const QString& macAddress);

	bool removeHosts(const QList<QModelIndex>& xx);

signals:
	void isEmptyChanged(bool isEmpty);
	void isIdleChanged(bool isIdle);

private:
	static QVariant displayName(const Wpp::HostPtr& host, int role);
	static QVariant hostname(const Wpp::HostPtr& host);
	static QVariant ipAddress(const Wpp::HostPtr& host);
	static QVariant macAddress(const Wpp::HostPtr& host);
	static QVariant lastSeenOnline(const Wpp::HostPtr& host);
	static QVariant opResult(const Wpp::HostPtr& host);
	void onHostDetected(int xHost);
	void onHostChanged(int xHost, Wpp::HostField field);
	void onIsIdleChanged(bool isIdle);
	void emitCellChanged(int xRow, Wpp::HostField field);
	[[nodiscard]] std::tuple<int, Wpp::HostField> getValidCell(const QModelIndex& x) const;

	template <typename Func, typename... Args>
	auto safeCall(Func f, Args&&... args)
		-> decltype(f(std::forward<Args>(args)...));

private:
	Wpp::NetworkPtr network;
	PersistorPtr persistor;

public:
	static constexpr const char* columnNames[] = {
		"Friendly name", "Hostname",
		"IP address", "MAC address",
		"Last seen online", "Wakeup"
	};

	enum Roles {
		LastSeenOnlineRole = Qt::UserRole + 1,
		StoredRole = Qt::UserRole + 2,
	};
};

template <typename Func, typename... Args>
// ReSharper disable once CppNotAllPathsReturnValue
auto ENetworkModel::safeCall(Func f, Args&&... args)
	-> decltype(f(std::forward<Args>(args)...))
{
	try {
		return f(std::forward<Args>(args)...);
	}
	catch (const std::system_error& ex) {
		std::stringstream ss;
		ss << "System error: " << ex.what() << " (code " << ex.code().value() << ")";
		qmlEngine(this)->throwError(tr(ss.str().c_str()));
	}
	catch (const std::exception& ex) {
		qmlEngine(this)->throwError(tr(ex.what()));
	}
}

#endif // NETWORK_MODEL_H
