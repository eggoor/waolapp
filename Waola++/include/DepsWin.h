#ifndef DEPSWIN_H
#define DEPSWIN_H

#include <WinSock2.h>
#include <WS2tcpip.h>

typedef unsigned short ushort;
typedef unsigned int uint;

inline int close(int s) { return closesocket(s); }

#endif // DEPSWIN_H
