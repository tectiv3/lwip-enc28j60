#include <mchdrv.h>
#include <lwip/pbuf.h>
#include <netif/etharp.h>
#include "enc28j60.h"

void mchdrv_poll(struct netif *netif) {
	// err_t result;
	struct pbuf *buf = NULL;
	enc_device_t *dev = (enc_device_t*)netif->state;
	int len = ENC28J60::packetReceive(dev, &buf);
	if (len) {
		// printf("received %d bytes\n", len);
		// result =
		netif->input(buf, netif);
		// printf("received with result %d\n", result);
	}
}

static err_t mchdrv_linkoutput(struct netif *netif, struct pbuf *buf) {
	ENC28J60::packetSend(buf);
	// printf("sent %d bytes.\n", buf->tot_len);
	return ERR_OK;
}

err_t mchdrv_init(struct netif *netif) {
	enc_device_t *dev = (enc_device_t*)netif->state;
	printf("Starting mchdrv_init.\n");

	dev->rxbufsize = ~0;
	dev->rdpt = ~0;

	uint8_t rev = ENC28J60::initialize(dev, 4*1024, netif->hwaddr);

	netif->output = etharp_output;
	netif->linkoutput = mchdrv_linkoutput;

	netif->mtu = 1500; /** FIXME check with documentation when jumboframes can be ok */

	netif->flags |= NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	printf("Driver initialized. Chip revision %d.\n", rev);
	return ERR_OK;
}
