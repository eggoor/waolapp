#ifndef VAULT_H
#define VAULT_H

#include <IVault.h>
#include <Host.h>
#include <VaultOpResult.h>

namespace Wpp {

class Vault final : public IVault
{
public:
    explicit Vault(const VaultParams& params);
    void init(std::vector<HostPtr>&& hosts) override;
	HostPtr operator[](size_t x) const override;
	[[nodiscard]] HostPtr at(size_t x) const override;
	[[nodiscard]] size_t hostCount() const override;
    [[nodiscard]] bool isEmpty() const override;
    VaultOpResult tryAdd(Host&& newHost) override;
	VaultOpResult tryUpdateL7(const L7Result& l7Res) override;
    [[nodiscard]] const std::vector<HostPtr>& getHosts() const override;
	void removeHosts(size_t xHost, size_t count) override;

private:
	mutable std::mutex mtx;
    std::vector<HostPtr> hosts;
};

} // namespace Wpp

#endif // VAULT_H
