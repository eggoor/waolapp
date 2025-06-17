#ifndef E_NETWORK_INTERFACE_PROVIDER_H
#define E_NETWORK_INTERFACE_PROVIDER_H

#include "e_net_iface.h"

#if __cplusplus
extern "C" {
#endif

/// <summary>
/// Retrieves a pointer to the first available network interface. Must be freed with the @e_iface_prov_free().
/// </summary>
/// <returns>A pointer to the first up and running network interface if any, otherwise NULL.</returns>
e_net_iface_t* const e_iface_prov_get_first();

/// <summary>
/// Retrieves the next network interface in the provider's list after the specified interface.
/// </summary>
/// <param name="iface">A pointer to the current network interface.</param>
/// <returns>A pointer to the next network interface, or NULL if there are no more interfaces.</returns>
e_net_iface_t* const e_iface_prov_get_next(const e_net_iface_t* const restrict_ iface);

/// <summary>
/// Frees the resources associated with a network interface provider.
/// </summary>
/// <param name="first">Pointer returned by the @e_iface_prov_get_first().</param>
void e_iface_prov_free(e_net_iface_t* restrict_ first);

#if __cplusplus
}   // Extern C
#endif

#endif // E_NETWORK_INTERFACE_PROVIDER_H
