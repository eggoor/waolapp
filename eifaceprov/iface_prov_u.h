#ifndef E_INTERFACE_PROVIDER_UNIX_H
#define E_INTERFACE_PROVIDER_UNIX_H

#include "common.h"

struct e_net_iface* e_iface_prov_make_iface_list(int socket, const struct ifconf* ifconf);
uint e_iface_prov_get_iface_index(int socket, const struct ifreq* restrict_ ifreq);
byte* e_iface_prov_get_mac_addr(int fd, const struct ifreq* restrict_ ifreq);

#endif // E_INTERFACE_PROVIDER_UNIX_H
