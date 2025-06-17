#ifndef IPERSISTOR_H
#define IPERSISTOR_H

#include <Host.h>

typedef std::unique_ptr<class IPersistor> PersistorPtr;

class IPersistor {
public:
    static PersistorPtr create();
    virtual void persist(const std::vector<Wpp::HostPtr>& hosts) = 0;
    virtual std::vector<Wpp::HostPtr> restore() = 0;
    virtual ~IPersistor() = default;
};

#endif // IPERSISTOR_H
