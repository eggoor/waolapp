#ifndef WIN_DEFS_H
#define WIN_DEFS_H

typedef unsigned short ushort;
typedef unsigned int uint;

inline int close(int s) { return closesocket(s); }

#endif // WIN_DEFS_H
