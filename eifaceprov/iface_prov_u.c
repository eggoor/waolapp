#include "iface_prov.h"

#include "iface_prov_u.h"

struct e_net_iface* e_iface_prov_collect_ifaces()
{
	static const int def_ifc_len = 0x1000;
	int ifc_len = def_ifc_len;
	struct ifconf ifc = { 0 };
    struct e_net_iface* first = NULL;
	
	char ch;
	ifc.ifc_buf = &ch;

	int s = socket(AF_INET, SOCK_DGRAM, 0);

	if (s < 0) {
		goto end;
	}

	ifc.ifc_len = ifc_len;

	while (1) {
		ifc.ifc_buf = malloc(ifc.ifc_len);

		if (!ifc.ifc_buf) {
            goto cleanup2;
		}

		if (ioctl(s, SIOCGIFCONF, &ifc) < 0) {
            goto cleanup1;
		}

		if (ifc.ifc_len < ifc_len) {
			break;
		}

		free(ifc.ifc_buf);
		ifc_len += def_ifc_len;
		ifc.ifc_len = ifc_len;
	}

	first = e_iface_prov_make_iface_list(s, &ifc);

cleanup1:
    free(ifc.ifc_buf);

cleanup2:
    e_iface_prov_close_socket(s);

end:
    return first;
}

struct e_net_iface* e_iface_prov_make_iface(int socket, const struct ifreq* const restrict_ master_ifreq)
{
	struct e_net_iface* iface = NULL;
	struct sockaddr* sa = NULL;

	sa = e_iface_prov_sockaddr_dup(&master_ifreq->ifr_addr);

	if (!sa)
		goto end;

	const uint ifindex = e_iface_prov_get_iface_index(socket, master_ifreq);

	uint ipv4_mask = 0;

	if (AF_INET == sa->sa_family) {
		struct ifreq ifreq;
		memset(&ifreq, 0, sizeof *master_ifreq);
		strncpy(ifreq.ifr_name, master_ifreq->ifr_name, IFNAMSIZ);

		if (ioctl(socket, SIOCGIFNETMASK, &ifreq) != 0)
			goto cleanup;

        ipv4_mask = e_net_iface_ipv4_sockaddr_to_int(&ifreq.ifr_addr);
	}

	byte* mac = e_iface_prov_get_mac_addr(socket, master_ifreq);

	if (mac) {
		iface = e_net_iface_create(ifindex, master_ifreq->ifr_name, sa, ipv4_mask, mac);
		free(mac);
	}

cleanup:
	free(sa);

end:
	return iface;
}

void e_iface_prov_close_socket(int socket) {
	close(socket);
}
