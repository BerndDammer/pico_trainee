#include "cmsis_gcc.h"
#include "rp2040_scheduler.h"

extern struct thread_info {
        struct thread_stack_frame *initial_stack;
    uint8_t *stack_base;
    uint32_t stack_size;

}thread_table[];