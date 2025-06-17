#ifndef E_INTERFACE_PROVIDER_H
#define E_INTERFACE_PROVIDER_H

#include "net_iface.h"

struct e_net_iface* e_iface_prov_collect_ifaces();
struct e_net_iface* e_iface_prov_make_iface(int socket, const struct ifreq* restrict_ master_ifreq);
void e_iface_prov_close_socket(int socket);

#endif // E_INTERFACE_PROVIDER_H
