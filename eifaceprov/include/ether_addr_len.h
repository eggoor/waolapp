#ifndef ETHERADDRLEN_H
#define ETHERADDRLEN_H

#if defined(__linux__) || defined(__APPLE__)
#include <net/ethernet.h>
#else
#define ETHER_ADDR_LEN 6
#endif

#endif // ETHERADDRLEN_H
