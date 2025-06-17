#ifndef E_INTERFACE_PROVIDER_PRIVATE_H
#define E_INTERFACE_PROVIDER_PRIVATE_H

#include <include/e_net_iface.h>

static struct e_net_iface* make_iface_list(int socket, const struct ifconf* restrict_ ifconf);
static struct e_net_iface* make_iface(int socket, const struct ifreq* restrict_ master_ifreq);
static int get_iface_index(int socket, const struct ifreq* restrict_ ifreq);
static byte* get_mac_addr(int fd, const struct ifreq* restrict_ ifreq);

#endif // E_INTERFACE_PROVIDER_PRIVATE_H
