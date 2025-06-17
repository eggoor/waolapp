#ifndef E_NETWORK_INTERFACE_H
#define E_NETWORK_INTERFACE_H

#if __cplusplus
extern "C" {
#endif

/**
 * \struct network interface.
 */
typedef struct e_net_iface e_net_iface_t;

uint e_net_iface_get_index(const e_net_iface_t* restrict_ iface);
const char* e_net_iface_get_name(const e_net_iface_t* restrict_ iface);
const struct sockaddr* e_net_iface_get_socket_address(const e_net_iface_t* restrict_ iface);
uint e_net_iface_get_ipv4_address(const struct sockaddr* restrict_ iface);
uint e_net_iface_get_ipv4_mask(const e_net_iface_t* iface);
const unsigned char* e_net_iface_get_mac_address(const e_net_iface_t* restrict_ iface);
int e_iface_prov_get_sockaddr_len(const struct sockaddr* restrict_ sa);

#if __cplusplus
}   // Extern C
#endif

#endif // E_NETWORK_INTERFACE_H
