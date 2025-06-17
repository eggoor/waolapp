#include "net_iface.h"
#include "helper.h"

#define DEF_BUF_SIZ 0x1000
#define DEF_TRY_COUNT 3

int e_iface_prov_get_sockaddr_len(const struct sockaddr* const restrict_ sa) {
	int sa_len = 0;

	if (!sa) {
		return 0;
	}

	switch (sa->sa_family) {
	case AF_INET: {
		sa_len = sizeof(struct sockaddr_in);
		break;
	}
	case AF_INET6: {
		sa_len = sizeof(struct sockaddr_in6);
		break;
	}
	default: {
		// Do nothing
	}
	}

	return sa_len;
}

struct e_net_iface* e_iface_prov_collect_ifaces()
{
	struct e_net_iface* first = NULL;
	struct e_net_iface* iface = NULL;

	int rc = 0;
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
	ULONG family = AF_UNSPEC;
	ULONG bus_siz = DEF_BUF_SIZ;
	int tries = 0;
	PIP_ADAPTER_ADDRESSES addresses = NULL;

	do {
		addresses = malloc(bus_siz);
		if (!addresses) {
			goto end;
		}

		rc = GetAdaptersAddresses(family, flags, NULL, addresses, &bus_siz);

		if (ERROR_BUFFER_OVERFLOW == rc) {
			free(addresses);
			addresses = NULL;
		}
		else {
			break;
		}
	} while (ERROR_BUFFER_OVERFLOW == rc && tries++ < DEF_TRY_COUNT);

	if (NO_ERROR != rc) {
		goto cleanup;
	}

	for (PIP_ADAPTER_ADDRESSES curr_addr = addresses; curr_addr; curr_addr = curr_addr->Next) {
		char* name = str_16_to_8(curr_addr->FriendlyName);

		if (curr_addr->IfType != IF_TYPE_SOFTWARE_LOOPBACK
			&& curr_addr->OperStatus == IfOperStatusUp
			&& (curr_addr->Ipv4Enabled || curr_addr->Ipv6Enabled)) {

			PIP_ADAPTER_UNICAST_ADDRESS unicast_addr = curr_addr->FirstUnicastAddress;

			while (unicast_addr) {
				ULONG ipv4mask = 0;

				if (AF_INET == unicast_addr->Address.lpSockaddr->sa_family
					&& ConvertLengthToIpv4Mask(unicast_addr->OnLinkPrefixLength, &ipv4mask) != NO_ERROR) {
					continue;
				}

				struct e_net_iface* prev = iface;
				iface = e_net_iface_create(curr_addr->IfIndex, name ? name : curr_addr->AdapterName,
					unicast_addr->Address.lpSockaddr, ntohl(ipv4mask), NULL);

				if (iface) {
					if (!first)
						first = iface;

					if (prev)
						prev->next = iface;
				}

				unicast_addr = unicast_addr->Next;
			}
		}

		free(name);
	}

cleanup:
	free(addresses);

end:
	return first;
}
