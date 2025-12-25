#include "project_main.h"
#include "rp2040_scheduler.h"
#include "thread_list.h"

extern unsigned int __scratch_x_start__;
extern unsigned int __scratch_x_end__;

void project_main(void)
{
    union
    {
        unsigned int xPSR;

        struct bitmap
        {
            unsigned int b0 : 1;
            unsigned int unused : 6;
            unsigned int b1 : 1;
        } bits;
    } a;
    
    a.xPSR = __scratch_x_start__;
    if (a.bits.b0)
    {
        a.bits.b1 ^= 1;
    }
    else
    {
        a.bits.b1++;
    }
    __scratch_x_end__ =a.xPSR;

}