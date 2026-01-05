#include <stdbool.h>
#include <stdlib.h>

#include "easydisplib.h"

typedef struct {
    edl_u32 width;
    edl_u32 height;
    edl_u32 position[2];
} object;


void exit_if_error(int err) {
    if (err == EXIT_SUCCESS) return;
    printf("Program abnormally exited with status: %d\n", err);
    exit(err);
}


int main() {

    // Error
    int err;

    // Declaration of a screen 50x50 characters
    EDL_SCREEN screen;
    const edl_u32 res_x = 800;
    const edl_u32 res_y = 600;

    // Colors
    const edl_u32 bg = 0xFF008800;
    const edl_u32 c1 = 0x88FF0000;
    const edl_u32 c2 = 0x880000FF;
    
    // Initialize screen with black
    err = edl_init_screen(&screen,
                          res_x,
                          res_y,
                          bg);
    exit_if_error(err);
    
    // Initialize object
    object obj1 = {res_x/20, res_y/20, {0,0}};
    object obj2 = {res_x/20, res_y/20, {res_x/40, res_y/40}};    

    // Create the sprites
    EDL_SPRITE line1;
    EDL_SPRITE line2;
    EDL_SPRITE line3;
    EDL_SPRITE tri1;
    EDL_SPRITE sq1;
    EDL_SPRITE sq2;
    
    // Initialize the sprites
    err = edl_init_sprite(&line1);
    err = edl_init_sprite(&line2);
    err = edl_init_sprite(&line3);
    err = edl_init_sprite(&tri1);        
    err = edl_init_sprite(&sq1);
    err = edl_init_sprite(&sq2);
    
    // Create sprites
    EDL_VEC2 p11 = {0,0},
             p12 = {800,600},
             p21 = {0,600},
             p22 = {800,0},
             p31 = {0,0},
             p32 = {100,0},
             v11 = {0,0},
             v12 = {0,30},
             v13 = {15,30};
    err = edl_line_sprite(&line1, p11, p12, 0xFF000000);
    err = edl_line_sprite(&line2, p21, p22, 0xFFFFFFFF);    
    err = edl_triangle_sprite(&tri1, v11, v12, v13, 0xFFFFFFFF);
    err = edl_square_sprite(&sq1, obj1.width, obj2.height, c1);
    err = edl_square_sprite(&sq2, obj2.width, obj2.height, c2);

    
    // Line3 position and color
    edl_u32 l3pos[] = {0,0};
    edl_u32 cl3 = 0xFF000000;
    
    // Loop
    bool exit = false;
    while(!exit) {
        
        // Clear the buffer
        err = edl_clear_screen(&screen, bg);
        exit_if_error(err);

        // Compute line3
        err = edl_line_sprite(&line3, p31, p32, cl3);
        
        // Write sprites in screen buffer
        err = edl_write_sprite_on_buffer(&screen, &line1, 0, 0);
        err = edl_write_sprite_on_buffer(&screen, &line2, 0, 0);
        err = edl_write_sprite_on_buffer(&screen, &line3, l3pos[0], l3pos[1]);
        err = edl_write_sprite_on_buffer(&screen, &tri1, res_x/3, res_y/3);        
        err = edl_write_sprite_on_buffer(&screen, &sq1, obj1.position[0], obj1.position[1]);
        err = edl_write_sprite_on_buffer(&screen, &sq2, obj2.position[0], obj2.position[1]);        

        // Show screen
        err = edl_show_screen(&screen);
        exit_if_error(err);

        // Deallocate line3
        err = edl_dalloc_sprite(&line3);
        
        // New position for objects
        obj1.position[0] += obj1.width;
        obj1.position[1] += obj1.height;
        obj2.position[0] += obj2.width;
        obj2.position[1] += obj2.height;
        l3pos[0] += obj1.width;
        l3pos[1] += obj1.height;
        // Update colors
        cl3 += 50;
        
        // Check exit criteria
        if (obj1.position[0] >= res_x-obj1.width/2) exit = true;
        if (obj1.position[1] >= res_y-obj1.height/2) exit = true;
    }
    
    // Free memory at the end of the execution
    err = edl_dalloc_screen(&screen);
    err = edl_dalloc_sprite(&line1);
    err = edl_dalloc_sprite(&line2);
    err = edl_dalloc_sprite(&tri1);    
    err = edl_dalloc_sprite(&sq1);
    err = edl_dalloc_sprite(&sq2);   
    exit_if_error(err);
    
    return 0;
}
