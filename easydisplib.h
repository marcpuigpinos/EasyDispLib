#ifndef EASYDISPLIB_H
#define EASYDISPLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define EDL_SUCCESS 0
#define EDL_FAILURE 1

#define u32 uint32_t

#define MAX_OUTPUT 9999
#define DIGITS 4

// Screen structure
typedef struct {
    u32 res_x;
    u32 res_y;
    u32 *buffer;
} EDL_SCREEN;

// Initialize EDL_SCREEN
int edl_init_screen(EDL_SCREEN *screen, const u32 res_x, const u32 res_y, u32 color);

// Deallocate EDL_SCREEN
int edl_dalloc_screen(EDL_SCREEN *screen);

// Show EDL_SCREEN
int edl_show_screen(const EDL_SCREEN *screen);

// clear EDL_SCREEN
int edl_clear_screen(EDL_SCREEN *screen, u32 color);

#endif
