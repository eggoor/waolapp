#include "iface_prov.h"

struct e_net_iface* e_iface_prov_get_first() {
    return e_iface_prov_collect_ifaces();
}

const struct e_net_iface* e_iface_prov_get_next(const struct e_net_iface* const restrict_ iface) {
	return iface ? iface->next : NULL;
}

void e_iface_prov_free(struct e_net_iface* restrict_ iface) {
    struct e_net_iface* restrict_ next = iface ? iface->next : NULL;
	while (next) {
        e_net_iface_free(iface);
		free(iface);
		iface = next;
		next = iface->next;
	}
}

char* e_iface_prov_strdup(const char* restrict_ src)
{
	char* dup = NULL;

	if (src && *src) {
		const size_t buf_size = strlen(src) + 1;
		dup = malloc(buf_size);
		if (dup) {
            char* dst = dup;
            while ((*dst++ = *src++));
		}
	}

	return dup;
}

struct sockaddr* e_iface_prov_sockaddr_dup(const struct sockaddr* const restrict_ sa)
{
	struct sockaddr* sa_dup = NULL;

    int sa_len = e_iface_prov_get_sockaddr_len(sa);

    if (sa_len == 0)
        goto end;

    sa_dup = malloc(sa_len);

    if (!sa_dup)
        goto end;

    memcpy(sa_dup, sa, sa_len);

end:
	return sa_dup;
}

uint e_net_iface_ipv4_sockaddr_to_int(const struct sockaddr* const restrict_ sa)
{
	uint addr = 0;

	switch (sa->sa_family)
	{
	case AF_INET:
		addr = ntohl(((const struct sockaddr_in*)sa)->sin_addr.s_addr);
		break;
	default:
		{
			// Do nothing
		}
	}

	return addr;
}
