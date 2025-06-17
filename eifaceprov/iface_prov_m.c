#include "iface_prov.h"
#include "net_iface.h"

#include "iface_prov_u.h"

struct e_net_iface* e_iface_prov_make_iface_list(int socket,
    const struct ifconf* ifconf)
{
	struct e_net_iface* first = NULL;
	struct e_net_iface* iface = NULL;
	struct ifreq* ifreq = ifconf->ifc_req;
	
	for (int i = 0; i < ifconf->ifc_len;) {
		struct e_net_iface* prev = iface;
		iface = e_iface_prov_make_iface(socket, ifreq);

        if (iface) {
            if (!first)
				first = iface;

            if (prev)
                prev->next = iface;
        }
		
		int ifcreq_len = IFNAMSIZ + ifreq->ifr_addr.sa_len;
		ifreq = (struct ifreq*)((char*)ifreq + ifcreq_len);
		i += ifcreq_len;
	}

	return first;
}

uint e_iface_prov_get_iface_index(int socket, const struct ifreq* const restrict_ ifreq)
{
	return if_nametoindex(ifreq->ifr_name);
}

byte* e_iface_prov_get_mac_addr(int fd, const struct ifreq* const restrict_ ifreq)
{
	byte* mac_addr = NULL;
	const char* if_name = ifreq->ifr_name;
	struct ifaddrs* if_list;

	if (getifaddrs(&if_list) == 0) {
		for (struct ifaddrs* ifaddr = if_list; ifaddr; ifaddr = ifaddr->ifa_next) {
			if ((ifaddr->ifa_addr->sa_family == AF_LINK)
				&& (strcmp(ifaddr->ifa_name, if_name) == 0)
				&& ifaddr->ifa_addr) {
				const struct sockaddr_dl* sdl = (const struct sockaddr_dl*)ifaddr->ifa_addr;
                mac_addr = malloc(ETHER_ADDR_LEN);
                if (!mac_addr)
                    goto cleanup;
				memcpy(mac_addr, (byte*)LLADDR(sdl), ETHER_ADDR_LEN);
				break;
			}
		}

cleanup:
        freeifaddrs(if_list);
	}
	
    return mac_addr;
}

int e_iface_prov_get_sockaddr_len(const struct sockaddr* const restrict_ sa) {
    return sa ? sa->sa_len : 0;
}
