/*
 * memtool.c
 *
 *  Created on: 12.03.2023
 *      Author: manni4
 */

#include <stdio.h>

extern void * __flash_binary_start;
extern void * __boot2_start__;
extern void * __boot2_end__;
extern void * __logical_binary_start;
extern void * __VECTOR_TABLE;
extern void * __vectors;
extern void * _fini;
extern void * __exidx_end;
extern void * __binary_info_start;
extern void * __binary_info_end;
extern void * __etext;


extern void * ram_vector_table;
extern void * __scratch_x_start__;
extern void * __scratch_x_end__;
extern void * __scratch_y_start__;
extern void * __scratch_y_end__;


//extern void * __data_rom_start__;
extern void * __data_start__;
extern void * __data_end__;
extern void * __bss_start__;
extern void * __bss_end__;
extern void * __end__;
extern void * end;

extern void * __flash_binary_end;
extern void * __StackLimit;
extern void * __StackOneTop;
extern void * __StackTop;
extern void * __StackOneBottom;
extern void * __StackBottom;

//extern void * data_cpy_table;
//extern void * __ram_text_source__;
//extern void * __ram_text_start__;
//extern void * __ram_text_end__;



void dump_memory(void)
{
    printf("__flash_binary_start : %#10X\n", &__flash_binary_start);
    printf("__boot2_start__ : %#10X\n", &__boot2_start__);
    printf("__boot2_end__ : %#10X\n", &__boot2_end__);
    printf("__logical_binary_start : %#10X\n", &__logical_binary_start);
    printf("__VECTOR_TABLE : %#10X\n", &__VECTOR_TABLE);
    printf("_fini : %#10X", &_fini);
    printf("__exidx_end : %#10X\n", &__exidx_end);
    printf("__vectors : %#10X\n", &__vectors);
    printf("__binary_info_start : %#10X\n", &__binary_info_start);
    printf("__binary_info_end : %#10X\n", &__binary_info_end);
    printf("__etext : %#10X\n", &__etext);

    printf("ram_vector_table : %#10X\n", &ram_vector_table);
    printf("__scratch_x_start__ : %#10X\n", &__scratch_x_start__);
    printf("__scratch_x_end__ : %#10X\n", &__scratch_x_end__);
    printf("__scratch_y_start__ : %#10X\n", &__scratch_y_start__);
    printf("__scratch_y_end__ : %#10X\n", &__scratch_y_end__);

    //printf("__data_rom_start__ : %8xd", __data_rom_start__);
    printf("__data_start__ : %#10X\n", &__data_start__);
    printf("__data_end__ : %#10X\n", &__data_end__);
    printf("__bss_start__ : %#10X\n", &__bss_start__);
    printf("__bss_end__ : %#10X\n", &__bss_end__);
    printf("__end__ : %#10X\n", &__end__);
    printf("end : %#10X\n", &end);

    printf("__flash_binary_end : %#10X\n", &__flash_binary_end);
    printf("__StackLimit : %#10X\n", &__StackLimit);
    printf("__StackOneTop : %#10X\n", &__StackOneTop);
    printf("__StackTop : %#10X\n", &__StackTop);
    printf("__StackOneBottom : %#10X\n", &__StackOneBottom);
    printf("__StackBottom : %#10X\n", &__StackBottom);

    //printf("data_cpy_table : %#10X\n", &data_cpy_table);
    //printf("__ram_text_source__ : %#10X\n", &__ram_text_source__);
    //printf("__ram_text_start__ : %#10X\n", &__ram_text_start__);
    //printf("__ram_text_end__ : %#10X\n", &__ram_text_end__);
}
