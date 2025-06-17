#ifndef PERSISTOR_H
#define PERSISTOR_H

#include "IPersistor.h"

class Persistor final : public IPersistor {
public:
    void persist(const std::vector<Wpp::HostPtr>& hosts) override;
    std::vector<Wpp::HostPtr> restore() override;

private:
    static QString getFilename(bool ensurePath = false);
    static QJsonArray toJson(const std::vector<Wpp::HostPtr>& hosts);
    static std::vector<Wpp::HostPtr> fromJson(const QJsonArray& array);

    static constexpr auto filename = "waolaq_hosts.json";

    static constexpr auto fieldDisplayName = "displayName";
    static constexpr auto fieldHostname = "hostname";
    static constexpr auto fieldIpAddr = "ipAddr";
    static constexpr auto fieldMacAddr = "macAddr";
    static constexpr auto fieldLastSeenOnline = "lastSeenOnline";
};

#endif // PERSISTOR_H
