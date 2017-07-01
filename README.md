ENC28J60 driver and lwIP port
=============================

This repository contains a generic ENC28J60 driver with hardware abstraction
and an lwIP network interface port. An example implementation for the hardware
abstraction is provided for the EnergyMicro Gecko microcontroller, on which the
example of an lwIP project using the driver can be run.

It provides all the functionality required for stable send/receive operation
(was tested against concurrent flood pings and TCP requests), but has room for
extension, especially with respect to error reporting and optimization.
(Current code relies on polling only, and utilizes neither DMA nor interrupts.)

ENC28J60 driver
---------------

If compiled with `-DENC28J60_USE_PBUF`, two small additional functions,
@ref enc_read_received_pbuf and @ref enc_transmit_pbuf, are added, which are
designed to use the lwIP pbuf memory management system. This is to keep the
interfaces simple.

lwIP port
---------

The lwIP netif implementation for the driver resides in lwip/netif, following
the naming structure of the lwIP project (at least I hope so).

It is suitable for lwIP version 1.4.1 and the current (as of 2013-01-22) 1.5
development version.

Due to the interfaces provided by the driver itself, it is rather minimal, and
only consists of a init and a polling routine (which, as the name implies, is
to be called as often as possible).

