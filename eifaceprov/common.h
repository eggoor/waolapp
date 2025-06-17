#ifndef E_NETWORK_INTERFACE_PROVIDER_COMMON_H
#define E_NETWORK_INTERFACE_PROVIDER_COMMON_H

typedef unsigned char byte;
char* e_iface_prov_strdup(const char* restrict_ src);
int e_iface_prov_get_sockaddr_len(const struct sockaddr* restrict_ sa);
struct sockaddr* e_iface_prov_sockaddr_dup(const struct sockaddr* restrict_ sa);
unsigned int e_net_iface_ipv4_sockaddr_to_int(const struct sockaddr* restrict_ sa);

#endif // E_NETWORK_INTERFACE_PROVIDER_COMMON_H
