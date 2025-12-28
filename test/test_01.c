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
    EDL_SPRITE sq1;
    EDL_SPRITE sq2;

    // Initialize the sprites
    err = edl_init_sprite(&sq1);
    err = edl_init_sprite(&sq2);

    // Create squares sprites
    err = edl_square_sprite(&sq1, obj1.width, obj2.height, c1);
    err = edl_square_sprite(&sq2, obj2.width, obj2.height, c2);
    
    // Loop
    bool exit = false;
    while(!exit) {
        
        // Clear the buffer
        err = edl_clear_screen(&screen, bg);
        exit_if_error(err);

        // Write sprites in screen buffer
        err = edl_write_sprite_on_buffer(&screen, &sq1, obj1.position[0], obj1.position[1]);
        err = edl_write_sprite_on_buffer(&screen, &sq2, obj2.position[0], obj2.position[1]);        

        // Show screen
        err = edl_show_screen(&screen);
        exit_if_error(err);
        
        // New position for objects
        obj1.position[0] += obj1.width;
        obj1.position[1] += obj1.height;
        obj2.position[0] += obj2.width;
        obj2.position[1] += obj2.height;        
        
        // Check exit criteria
        if (obj1.position[0] >= res_x-obj1.width/2) exit = true;
        if (obj1.position[1] >= res_y-obj1.height/2) exit = true;
    }
    
    // Free memory at the end of the execution
    err = edl_dalloc_screen(&screen);
    exit_if_error(err);
    
    return 0;
}
