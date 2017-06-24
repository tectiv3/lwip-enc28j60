#ifndef NETIF_MCHDRV_H
#define NETIF_MCHDRV_H

/** lwIP interface handling for lwIP */

#include <lwip/netif.h>
#include <lwip/err.h>

#if defined(__cplusplus)
	extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif

/** netif init function; have this called by passing it to netif_add, along
 * with a pointer to an uninitialized enc_device_t state. The MAC address has
 * to be configured beforehand in the netif, and configured on the card. */
err_t mchdrv_init(struct netif *netif);
/** Call this in the main loop */
void mchdrv_poll(struct netif *netif);

#if defined(__cplusplus)
}
#endif
 
#endif
