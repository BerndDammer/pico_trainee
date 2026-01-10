#ifndef RP2040_SCHEDULER_LOGIC
#define RP2040_SCHEDULER_LOGIC

#include "rp2040_scheduler_stubs.h"

void sl_init(void);
void sl_first(int core_number, stack_pointer_t psp);
void sl_new(stack_pointer_t psp);
stack_pointer_t sl_next(int core_number, stack_pointer_t psp);
void sl_end(int core_number);

#endif
