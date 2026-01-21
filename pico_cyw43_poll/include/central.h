#include "pico/async_context_poll.h"
#include "cyw43.h"

typedef enum
{
    UNINIT = 0,
    INITIALIZED = 1,
    AP_ACTIVATED,
    AP_CONNECTED,
    STA_ACTIVATED,
    STA_CONNECTED,
    SCAN_RUNNING
} status_t;

typedef struct
{
    async_context_poll_t ac;
    cyw43_t *cyw43_self;
    status_t status;
} central_t;

extern central_t central;
