#define DEBUG_INIT              \
    {                           \
        gpio_init(16);          \
        gpio_init(17);          \
        gpio_set_dir(16, true); \
        gpio_set_dir(17, true); \
    }
#define DEBUG_SYS_TICK                             \
    {                                              \
        gpio_xor_mask(1 << (16 + get_core_num())); \
    }

#define DEBUG_PEND_SV           \
    {                           \
        gpio_xor_mask(1 << (16+ get_core_num())); \
    }
