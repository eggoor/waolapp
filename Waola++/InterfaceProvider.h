#ifndef INTERFACE_PROVIDER_H
#define INTERFACE_PROVIDER_H

#include <IInterfaceProvider.h>

namespace Wpp {

class InterfaceProvider final : public IInterfaceProvider
{
public:
	network_interfaces_t getInterfaces() const override;

private:
    static network_interfaces_t EnumInterfaces();
};

} // namespace Wpp

#endif // INTERFACE_PROVIDER_H
