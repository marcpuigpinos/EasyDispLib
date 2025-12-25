#ifndef EASYDISPLIB_H
#define EASYDISPLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define EDL_SUCCESS 0
#define EDL_FAILURE 1

#define edl_u32 uint32_t

/** DATA STRUCTURES **/

// Screen structure
typedef struct {
    edl_u32 res_x;
    edl_u32 res_y;
    edl_u32 *buffer;
} EDL_SCREEN;

// Sprite structure
typedef struct {
    edl_u32 res_x;
    edl_u32 res_y;
    edl_u32 *img;
} EDL_SPRITE;


/** EDL_SCREEN PROCEDURES **/

// Initialize EDL_SCREEN
int edl_init_screen(EDL_SCREEN *screen, const edl_u32 res_x, const edl_u32 res_y, edl_u32 color);

// Deallocate EDL_SCREEN
int edl_dalloc_screen(EDL_SCREEN *screen);

// Show EDL_SCREEN
int edl_show_screen(const EDL_SCREEN *screen);

// Clear EDL_SCREEN
int edl_clear_screen(EDL_SCREEN *screen, edl_u32 color);

/** END EDL_SCREEN PROCEDURES **/


/** EDL_SPRITE PROCEDURES **/

// Initialize sprite
int edl_init_sprite(EDL_SPRITE *sprite);

// Deallocate sprite
int edl_dalloc_sprite(EDL_SPRITE *sprite);

// Load an image to the sprite
int edl_load_sprite(EDL_SPRITE *sprite, char *filepath);

/** END EDL_SPRITE PROCEDURES **/

#endif
