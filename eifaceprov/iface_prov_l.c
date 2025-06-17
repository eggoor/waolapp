#include "iface_prov.h"
#include "net_iface.h"

#include "iface_prov_u.h"

struct e_net_iface* e_iface_prov_make_iface_list(int socket, const struct ifconf* ifconf)
{
    struct e_net_iface* first = NULL;
    struct e_net_iface* iface = NULL;

    struct ifreq* ifreq = ifconf->ifc_req;
    const int if_len = sizeof *ifreq;

    for (int i = 0; i < ifconf->ifc_len;)	{
        struct e_net_iface* prev = iface;
        iface = e_iface_prov_make_iface(socket, ifreq);

        if (iface) {
            if (!first)
                first = iface;

            if (prev)
                prev->next = iface;
        }

        ifreq = (struct ifreq*)((char*)ifreq + if_len);
        i += if_len;
    }

    return first;
}

int e_iface_prov_get_sockaddr_len(const struct sockaddr* const restrict_ sa) {
    int sa_len = 0;

    if (!sa)
        return 0;

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

uint e_iface_prov_get_iface_index(int socket, const struct ifreq* const restrict_ ifreq)
{
    int ifindex = -1;

    if (ioctl(socket, SIOCGIFINDEX, &ifreq) != 0) {
        ifindex = ifreq->ifr_ifindex;
    }

    return ifindex;
}

byte* e_iface_prov_get_mac_addr(int socket, const struct ifreq* ifreq)
{
    byte* mac_addr = NULL;

    if (ioctl(socket, SIOCGIFHWADDR, ifreq) == 0) {
        mac_addr = malloc(ETHER_ADDR_LEN);
        if (mac_addr)
            memcpy(mac_addr, (const byte*)ifreq->ifr_hwaddr.sa_data, ETHER_ADDR_LEN);

    }

    return mac_addr;
}
