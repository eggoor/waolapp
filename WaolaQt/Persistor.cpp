#include "Persistor.h"

PersistorPtr IPersistor::create()
{
    return std::make_unique<Persistor>();
}

void Persistor::persist(const std::vector<Wpp::HostPtr>& hosts)
{
    const auto fullFilename = getFilename(true);
    QFile file(fullFilename);

    if (!file.open(QIODevice::WriteOnly)) {
		throw std::runtime_error(("Cannot open file '" + fullFilename + '\'')
			.toStdString());
    }

    const QJsonDocument doc(toJson(hosts));
    file.write(doc.toJson());
}

std::vector<Wpp::HostPtr> Persistor::restore()
{
    const auto fullFilename = getFilename();
    QFile file(fullFilename);

    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }

    const auto doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray()) {
        return {};
    }

    return fromJson(doc.array());
}

QString Persistor::getFilename(const bool ensurePath)
{
    const auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (ensurePath) {
        const QDir dir;

        if (!dir.mkpath(path)) {
			throw std::runtime_error(("Unable to make path '" + path + '\'')
				.toStdString());
        }
    }

    return path.isEmpty() ? filename : QDir(path).filePath(filename);
}

QJsonArray Persistor::toJson(const std::vector<Wpp::HostPtr>& hosts)
{
    QJsonArray arr;

    for (const auto& host : hosts) {
        QJsonObject obj;
        obj[fieldDisplayName] = host->DisplayName.c_str();
        obj[fieldHostname] = host->Hostname.c_str();
        obj[fieldIpAddr] = static_cast<int>(static_cast<uint>(host->IpAddress));

        QJsonArray arrMac;
        for (int i = 0; i < ETHER_ADDR_LEN; ++i) {
            arrMac.append(host->MacAddress[i]);
        }
        obj[fieldMacAddr] = arrMac;

        obj[fieldLastSeenOnline] = static_cast<qint64>(host->LastSeenOnline);

        arr.append(obj);
    }

    return arr;
}

std::vector<Wpp::HostPtr> Persistor::fromJson(const QJsonArray& array)
{
    std::vector<Wpp::HostPtr> hosts;

    for (const auto& value : array) {
        QJsonObject obj = value.toObject();

        Wpp::MacAddress macAddr;
        const auto arrMac = obj[fieldMacAddr].toArray();

        int i = 0;
        for (const auto& byte : arrMac) {
            macAddr[i++] = byte.toInt();
        }

        hosts.emplace_back(std::make_unique<Wpp::Host>(
            obj[fieldDisplayName].toString().toStdString()
            , obj[fieldHostname].toString().toStdString()
            , Wpp::IpAddress(obj[fieldIpAddr].toInt())
            , macAddr
            , obj[fieldLastSeenOnline].toInteger()
        ));
    }

    return hosts;
}
