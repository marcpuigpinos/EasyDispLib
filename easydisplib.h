#ifndef EASYDISPLIB_H
#define EASYDISPLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <linux/fb.h>

#define EDL_SUCCESS 0
#define EDL_FAILURE 1

#define edl_u32 uint32_t

/** DATA STRUCTURES **/

// Struct vector 2 dim
typedef struct {
    edl_u32 x; // Component x of the vector
    edl_u32 y; // Component y of the vector
} EDL_VEC2;

typedef struct {
    int fd;                         // Frame buffer file descriptor
    char *mem;                      // Framebuffer pointer, to memory
    struct fb_var_screeninfo vinfo; // variable screen info
    struct fb_fix_screeninfo finfo; // fixed screen info
    long int screensize;            // Size of the sceen in bytes
} EDL_FB;

// Screen structure
typedef struct {
    edl_u32 res_x; // Width in pixels of the screen
    edl_u32 res_y; // Height in pixels of the screen
    edl_u32 *buffer;
} EDL_SCREEN;

// Sprite structure
typedef struct {
    edl_u32 width; // Width in pixels of the square
    edl_u32 height; // Height in pixels of the square
    edl_u32 *img;
} EDL_SPRITE;

/** END DATA STRUCTURES **/

/** EDL_COLOR PROCEDURES **/

// From hexa to rgba
int edl_from_hexa_to_rgba(const edl_u32 color,
                          unsigned char *r,
                          unsigned char *g,
                          unsigned char *b,
                          unsigned char *a);

// From rgba to hexa
int edl_from_rgba_to_hexa(const unsigned char r,
                          const unsigned char g,
                          const unsigned char b,
                          const unsigned char a,
                          edl_u32 *color);

// Mix color
int edl_mix_color(const edl_u32 cf, // Color foreground
                  const edl_u32 cb, // Color background
                  edl_u32 *cp);     // Color pixel

/** END EDL_COLOR PROCEDURES **/

/** EDL_FB PROCEDURES **/

// Open Linux framebuffer
int edl_open_fb();

// Close Linux framebuffer
int edl_close_fb();

// Write data on Linux framebuffer
int edl_write_fb();

/** END EDL_FB PROCEDURES **/

/** EDL_SCREEN PROCEDURES **/

// Initialize EDL_SCREEN
int edl_init_screen(EDL_SCREEN *screen,
                    const edl_u32 res_x,
                    const edl_u32 res_y,
                    edl_u32 color);

// Deallocate EDL_SCREEN
int edl_dalloc_screen(EDL_SCREEN *screen);

// Show EDL_SCREEN
int edl_show_screen_PAM(const EDL_SCREEN *screen);
int edl_show_screen_FB(const EDL_SCREEN *screen, EDL_FB *fb);

// Clear EDL_SCREEN
int edl_clear_screen(EDL_SCREEN *screen,
                     edl_u32 color);

// Write sprite on buffer
int edl_write_sprite_on_buffer(EDL_SCREEN *screen,
                               const EDL_SPRITE *sprite,
                               const edl_u32 pos_x,
                               const edl_u32 pos_y);

/** END EDL_SCREEN PROCEDURES **/


/** EDL_SPRITE PROCEDURES **/

// Initialize sprite
int edl_init_sprite(EDL_SPRITE *sprite);

// Deallocate sprite
int edl_dalloc_sprite(EDL_SPRITE *sprite);

// Line sprite
 int edl_line_sprite(EDL_SPRITE *sprite,
                     const EDL_VEC2 p1,
                     const EDL_VEC2 p2,
                     const edl_u32 color);

// Triangle sprite
int edl_triangle_sprite(EDL_SPRITE *sprite,
                        const EDL_VEC2 v1,
                        const EDL_VEC2 v2,
                        const EDL_VEC2 v3,
                        const edl_u32 color);

// Square sprite
int edl_square_sprite(EDL_SPRITE *sprite,
                      const edl_u32 width,
                      const edl_u32 height,
                      const edl_u32 color);

// Circle sprite
int edl_circle_sprite(EDL_SPRITE *sprite,
                      const edl_u32 radius,
                      const edl_u32 color);


// Load an image to the sprite
int edl_load_sprite(EDL_SPRITE *sprite,
                    char filepath[]);

/** END EDL_SPRITE PROCEDURES **/

#endif
