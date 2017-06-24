#include <mchdrv.h>
#include <lwip/pbuf.h>
#include <netif/etharp.h>
#include "enc28j60.h"

void mchdrv_poll(struct netif *netif) {
	err_t result;
	struct pbuf *buf = NULL;

	// uint8_t epktcnt;
	enc_device_t *dev = (enc_device_t*)netif->state;

	// epktcnt = enc_RCR(dev, ENC_EPKTCNT);
	// epktcnt = ENC28J60::get_packet_count();

	// if (epktcnt) {
		int len = ENC28J60::packetReceive(dev, &buf);
		// printf("incoming: %d packages, first read into %x\n", epktcnt, (unsigned int)(buf));
		if (len) {
			printf("received %d bytes\n", len);
			result = netif->input(buf, netif);
			printf("received with result %d\n", result);
		}
	// }
}

static err_t mchdrv_linkoutput(struct netif *netif, struct pbuf *buf) {
	// enc_device_t *dev = (enc_device_t*)netif->state;
	ENC28J60::packetSend(buf);
	printf("sent %d bytes.\n", buf->tot_len);
	/* FIXME: evaluate result state */
	return ERR_OK;
}

err_t mchdrv_init(struct netif *netif) {
	enc_device_t *dev = (enc_device_t*)netif->state;
	printf("Starting mchdrv_init.\n");

	// dev->last_used_register = ENC_BANK_INDETERMINATE;
	dev->rxbufsize = ~0;
	dev->rdpt = ~0;

	uint8_t rev = ENC28J60::initialize(dev, 4*1024, netif->hwaddr);

	netif->output = etharp_output;
	netif->linkoutput = mchdrv_linkoutput;

	netif->mtu = 1500; /** FIXME check with documentation when jumboframes can be ok */

	netif->flags |= NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	printf("Driver initialized. Revision %d.\n", rev);
	return ERR_OK;
}
