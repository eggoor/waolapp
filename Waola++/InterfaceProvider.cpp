#include <e_iface_prov.h>

#include "NetworkInterface.h"

#include "InterfaceProvider.h"

namespace Wpp {
	InterfaceProviderPtr IInterfaceProvider::make()
	{
		return std::make_unique<InterfaceProvider>();
	}

	network_interfaces_t InterfaceProvider::getInterfaces() const
	{
		return EnumInterfaces();
	}

	network_interfaces_t InterfaceProvider::EnumInterfaces()
	{
		network_interfaces_t ifaces;

		const auto first = e_iface_prov_get_first();
		auto next = first;

		while (next) {
			auto index = e_net_iface_get_index(next);
			const auto name = e_net_iface_get_name(next);
			const auto sa = e_net_iface_get_socket_address(next);
			auto ipv4Mask = e_net_iface_get_ipv4_mask(next);
			const auto macAddr = reinterpret_cast<const byte*>(e_net_iface_get_mac_address(next));

			const auto pair = ifaces.emplace(index, std::vector<NetworkInterfacePtr>());
			pair.first->second.emplace_back(std::make_unique<NetworkInterface>(index, name, sa, ipv4Mask, macAddr));

			next = e_iface_prov_get_next(next);
		}

		e_iface_prov_free(first);
		return ifaces;
	}
} // namespace Wpp
