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
		printf("received %d bytes\n", len);
		result = netif->input(buf, netif);
		printf("received with result %d\n", result);
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
	// int result;
	enc_device_t *dev = (enc_device_t*)netif->state;

	printf("Starting mchdrv_init.\n");

	// result = enc_setup_basic(dev);
	// if (result != 0)
	// {
	// 	printf("Error %d in enc_setup, interface setup aborted.\n", result));
	// 	return ERR_IF;
	// }
	// result = enc_bist_manual(dev);
	// if (result != 0)
	// {
	// 	printf("Error %d in enc_bist_manual, interface setup aborted.\n", result));
	// 	return ERR_IF;
	// }
	// dev->last_used_register = ENC_BANK_INDETERMINATE;
	dev->rxbufsize = ~0;
	dev->rdpt = ~0;

	ENC28J60::initialize(dev, 4*1024, netif->hwaddr);
	// enc_ethernet_setup(dev, 4*1024, netif->hwaddr);

	netif->output = etharp_output;
	netif->linkoutput = mchdrv_linkoutput;

	netif->mtu = 1500; /** FIXME check with documentation when jumboframes can be ok */

	netif->flags |= NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

	printf("Driver initialized.\n");

	return ERR_OK;
}
