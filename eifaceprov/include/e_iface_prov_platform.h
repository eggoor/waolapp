#ifndef E_IFACE_PROV_PLATFORM_H
#define E_IFACE_PROV_PLATFORM_H

#ifdef __cplusplus
#define restrict_
#else
#define restrict_ restrict
#endif

#if defined(_WIN32)
	#include "e_iface_prov_win.h"
#endif

#endif // E_IFACE_PROV_PLATFORM_H
