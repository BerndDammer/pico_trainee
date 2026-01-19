#include "cyw43.h"



void cyw43_delay_ms(long unsigned int millies)
{
    
}
void cyw43_delay_us(long unsigned int millies)
{
    
}
void cyw43_thread_enter(void)
{

}
void cyw43_thread_exit(void)
{

}
void cyw43_schedule_internal_poll_dispatch(void (*func)(void))
{

}
void cyw43_await_background_or_timeout_us(uint32_t timeout)
{

}
void cyw43_post_poll_hook(void)
{

}
void cyw43_hal_generate_laa_mac(int i, uint8_t *mac)
{

}

/////////////////////////////////////////////////////////////////////
////////////////////////////// callbacks for network stack
void cyw43_cb_tcpip_init(cyw43_t *self, int itf) 
{

}
void cyw43_cb_tcpip_deinit(cyw43_t *self, int itf) 
{
}
void cyw43_cb_tcpip_set_link_up(cyw43_t *self, int itf) 
{
}
void cyw43_cb_tcpip_set_link_down(cyw43_t *self, int itf) 
{
}
void cyw43_cb_process_ethernet(void *cb_data,int itf, size_t len, const uint8_t *buf) 
{
}


