#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <e_iface_prov_platform.h>

#include "common.h"

#if __cplusplus
extern "C" {
#endif

struct e_net_iface {
	char* displayName;
	struct sockaddr* sockAddr;
	uint index;
	uint ipv4mask;
	byte macAddr[ETHER_ADDR_LEN];
	struct e_net_iface* next;
};

struct e_net_iface* e_net_iface_create(uint index, const char* name, const struct sockaddr* restrict_ sa,
	uint ipv4mask, const byte* restrict_ mac_addr);
void e_net_iface_free(struct e_net_iface* iface);

#if __cplusplus
}   // Extern C
#endif

#endif // NETWORK_INTERFACE_H
