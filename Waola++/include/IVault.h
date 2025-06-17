#ifndef IVAULT_H
#define IVAULT_H

#include "Host.h"
#include "L7Result.h"
#include "VaultParams.h"
#include "VaultOpResult.h"

namespace Wpp {

typedef std::unique_ptr<class IVault> VaultPtr;
typedef std::function<VaultPtr(const VaultParams&)> FVaultFactory;

class IVault {
public:
    static VaultPtr create(const VaultParams& params);
    virtual void init(std::vector<Wpp::HostPtr>&& hosts) = 0;
	virtual HostPtr operator[](size_t x) const = 0;
	virtual HostPtr at(size_t x) const = 0;
	virtual size_t hostCount() const = 0;
    virtual bool isEmpty() const = 0;
    virtual VaultOpResult tryAdd(Host&& newHost) = 0;
    virtual VaultOpResult tryUpdateL7(const L7Result& l7Res) = 0;
    virtual const std::vector<HostPtr>& getHosts() const = 0;
	virtual void removeHosts(size_t xHost, size_t count) = 0;
    virtual ~IVault() = default;
};

} // namespace Wpp

#endif // IVAULT_H
