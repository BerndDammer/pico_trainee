#include "cyw43.h"


/////////////////////////////////////////////////////////////////////
////////////////////////////// callbacks for network stack
void cyw43_cb_tcpip_init(cyw43_t *self, int itf) 
{
    puts("CB init\n");
}
void cyw43_cb_tcpip_deinit(cyw43_t *self, int itf) 
{
    puts("CB de init\n");
}
void cyw43_cb_tcpip_set_link_up(cyw43_t *self, int itf) 
{
    puts("CB de link up\n");
}
void cyw43_cb_tcpip_set_link_down(cyw43_t *self, int itf) 
{
    puts("CB de link down\n");
}
void cyw43_cb_process_ethernet(void *cb_data,int itf, size_t len, const uint8_t *buf) 
{
    puts("CB ethernet frame\n");   
}
int cyw43_tcpip_link_status(cyw43_t *self, int itf)
{
    puts("CB status\n");   
}

