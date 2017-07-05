#include <mchdrv.h>
#include <lwip/pbuf.h>
#include <lwip/stats.h>
#include <netif/etharp.h>
#include "enc28j60.h"

void mchdrv_poll(struct netif *netif) {
	// err_t result;
	struct pbuf *buf = NULL;
	struct eth_hdr *ethhdr;
	enc_device_t *dev = (enc_device_t*)netif->state;
	int len = ENC28J60::packetReceive(dev, &buf);
	if (len) {
		// printf("received %d bytes\n", len);
	    LINK_STATS_INC(link.recv);
		ethhdr = (eth_hdr*)buf->payload;
		switch (htons(ethhdr->type)) {
			/* IP or ARP packet? */
			case ETHTYPE_IP:
			case ETHTYPE_IPV6:
			case ETHTYPE_ARP:
				/* full packet send to tcpip_thread to process */
				if (netif->input(buf, netif) != ERR_OK) {
					printf("mchdrv_poll: IP input error\n");
					pbuf_free(buf);
					buf = NULL;
				}
				break;
			default:
				pbuf_free(buf);
				buf = NULL;
				break;
			}
		// result =
		// netif->input(buf, netif);
		// printf("received with result %d\n", result);
	}
}

static err_t mchdrv_linkoutput(struct netif *netif, struct pbuf *buf) {
	ENC28J60::packetSend(buf);
	// printf("sent %d bytes.\n", buf->tot_len);
    LINK_STATS_INC(link.xmit);
	return ERR_OK;
}

err_t mchdrv_init(struct netif *netif) {
	enc_device_t *dev = (enc_device_t*)netif->state;
	printf("Starting mchdrv_init. Interface num: %d.\n", netif->num);

	dev->rxbufsize = ~0;
	dev->rdpt = ~0;

	uint8_t rev = ENC28J60::initialize(dev, 4*1024, netif->hwaddr);

	netif->output = etharp_output;
	netif->linkoutput = mchdrv_linkoutput;

	netif->mtu = 1500;

	netif->flags |= NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	printf("Driver initialized. Chip revision %d.\n", rev);
	return ERR_OK;
}
