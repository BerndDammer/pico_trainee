#include "cyw43.h"
#include "central.h"

const char *itf_txt[2] =
    {
        "STATION      :",
        "ACCESS POINT :" //
};
/////////////////////////////////////////////////////////////////////
static void monitoring(const char *message, cyw43_t *self, int itf)
{
    if (central.cyw43_self == NULL)
    {
        central.cyw43_self = self;
        puts("loaded internal struct\n");
    }
    printf("%s %s\n", itf_txt[itf], message);
}
////////////////////////////// callbacks for network stack
void cyw43_cb_tcpip_init(cyw43_t *self, int itf)
{
    monitoring(__FUNCTION__, self, itf);
}
void cyw43_cb_tcpip_deinit(cyw43_t *self, int itf)
{
    monitoring(__FUNCTION__, self, itf);
}
void cyw43_cb_tcpip_set_link_up(cyw43_t *self, int itf)
{
    monitoring(__FUNCTION__, self, itf);
}
void cyw43_cb_tcpip_set_link_down(cyw43_t *self, int itf)
{
    monitoring(__FUNCTION__, self, itf);
}

volatile uint8_t p[256];
void cyw43_cb_process_ethernet(void *cb_data, int itf, size_t len, const uint8_t *buf)
{
    for (int i = 0; i < 256; i++)
    {
        p[i] = buf[i];
    }
    printf("CB ethernet frame itf %i\n", itf);
    printf("%s  FrameType %04X\n", itf_txt[itf], p[12] << 8 | p[13]); // network byte order
}

int cyw43_tcpip_link_status(cyw43_t *self, int itf)
{
    monitoring(__FUNCTION__, self, itf);

    puts("CB tcpip link status\n");
    printf("State %i\n", self->itf_state);
    /// QQQ endless looop
    printf("cyw43_wifi_link_status STA %i\n", cyw43_wifi_link_status(self, CYW43_ITF_STA));
    printf("cyw43_wifi_link_status AP  %i\n", cyw43_wifi_link_status(self, CYW43_ITF_AP));
}

// workaround:
// dummy implementation because used in cyw43_ll send_ethernet
// part of not used lwip
void pbuf_copy_partial(void *buf, int a, int b, int c)
{
}
