#ifndef IINTERFACEPROVIDER_H
#define IINTERFACEPROVIDER_H

namespace Wpp {
	typedef std::unique_ptr<class NetworkInterface> NetworkInterfacePtr;
	typedef std::map<int, std::vector<NetworkInterfacePtr>> network_interfaces_t;
	typedef std::unique_ptr<class IInterfaceProvider> InterfaceProviderPtr;

	class IInterfaceProvider {
	public:
		static InterfaceProviderPtr make();
		virtual network_interfaces_t getInterfaces() const = 0;
		virtual ~IInterfaceProvider() = default;
	};
} // namespace Wpp

#endif //IINTERFACEPROVIDER_H
