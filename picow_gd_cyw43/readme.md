File 
#include "cyw43_config.h"
must be given by implementer

#if CYW43_LWIP
#if CYW43_NETUTILS
itf = CYW43_ITF_STA or CYW43_ITF_AP


cyw43.h
wants
cyw43_config.h
wants
cyw43_configport.h

CYW43_USE_OTP_MAC set to true
get internal mac

CYW43_LOGIC_DEBUG 1 flagt pins 2-10

Compile only one of

#if !CYW43_USE_SPI
cyw43_spi.c 

#if CYW43_USE_SPI
cyw43_sdio.c

-----------------------


#if's in cyw43_lwip.c
CYW43_HOST_NAME
LWIP_IPV4
LWIP_IPV6
NO_SYS
LWIP_DNS
LWIP_DHCP
??? cyw43_lwip.c not compile ... gives network interface ?????
-----------------------
CYW43_USE_OTP_MAC should be (1) to get from otp in wlan module
----------------------------
CYW43_NETUTILS set to (0) we already plan to take all raw packages
-------------------------



