#include "cyw43.h"
#include "central.h"

/////////////////////////////////////////////////////////////////////
////////////////////////////// callbacks for network stack
void cyw43_cb_tcpip_init(cyw43_t *self, int itf)
{
    puts("CB init\n");
    if(central.cyw43_self == NULL)
    {
        central.cyw43_self = self;
        puts("loaded internal struct\n");
    }
}
void cyw43_cb_tcpip_deinit(cyw43_t *self, int itf)
{
    puts("CB de-init\n");
}
void cyw43_cb_tcpip_set_link_up(cyw43_t *self, int itf)
{
    puts("CB link up\n");
    if(central.cyw43_self == NULL)
    {
        central.cyw43_self = self;
        puts("loaded internal struct\n");
    }
}
void cyw43_cb_tcpip_set_link_down(cyw43_t *self, int itf)
{
    puts("CB link down\n");
}


volatile uint8_t p[256];
void cyw43_cb_process_ethernet(void *cb_data, int itf, size_t len, const uint8_t *buf)
{
    for (int i = 0; i < 256; i++)
    {
        p[i] = buf[i];
    }
    puts("CB ethernet frame");
    printf("FrameType %04X\n", p[12]<<8|p[13]); //network byte order
}

int cyw43_tcpip_link_status(cyw43_t *self, int itf)
{
    if(central.cyw43_self == NULL)
    {
        central.cyw43_self = self;
        puts("loaded internal struct\n");
    }
    puts("CB status\n");
    printf("State %i\n", self->itf_state);
    printf("cyw43_wifi_link_status %i\n", cyw43_wifi_link_status(self, CYW43_ITF_STA));
}
