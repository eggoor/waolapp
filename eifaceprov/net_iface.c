#include "net_iface.h"

struct e_net_iface* e_net_iface_create(const uint index, const char* name, const struct sockaddr* const restrict_ sa,
	const uint ipv4mask, const byte* const restrict_ mac_addr)
{
	struct e_net_iface* iface = malloc(sizeof(struct e_net_iface));

	if (!iface) {
		goto end;
	}

	iface->next = NULL;

	iface->displayName = e_iface_prov_strdup(name);

	iface->sockAddr = e_iface_prov_sockaddr_dup(sa);

	iface->index = index;
	iface->ipv4mask = ipv4mask;

	if (mac_addr) {
		memcpy(iface->macAddr, mac_addr, ETHER_ADDR_LEN);
	}
	else {
		memset(iface->macAddr, 0, ETHER_ADDR_LEN);
	}

end:
	return iface;
}

void e_net_iface_free(struct e_net_iface* iface) {
	if (!iface) {
        return;
	}

    free(iface->displayName);
    free(iface->sockAddr);
}

int e_net_iface_get_address_family(const struct e_net_iface* const restrict_ iface) {
    return iface ? iface->sockAddr->sa_family : AF_UNSPEC;
}

uint e_net_iface_get_index(const struct e_net_iface* const restrict_ iface) {
    return iface ? iface->index : 0;
}

const char* e_net_iface_get_name(const struct e_net_iface* const restrict_ iface) {
    return iface ? iface->displayName : NULL;
}

const struct sockaddr* e_net_iface_get_socket_address(const struct e_net_iface* const restrict_ iface) {
	return iface ? iface->sockAddr : NULL;
}

uint e_net_iface_get_ipv4_address(const struct e_net_iface* const restrict_ iface) {
    return iface ? e_net_iface_ipv4_sockaddr_to_int(iface->sockAddr) : 0;
}

uint e_net_iface_get_ipv4_mask(const struct e_net_iface* const restrict_ iface) {
    return iface ? iface->ipv4mask : 0;
}

const unsigned char* e_net_iface_get_mac_address(const struct e_net_iface* const restrict_ iface) {
    return iface ? iface->macAddr : NULL;
}
