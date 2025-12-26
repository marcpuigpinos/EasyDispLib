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

    // Initialize screen with black
    err = edl_init_screen(&screen,
                          res_x,
                          res_y,
                          0xFF000000);
    exit_if_error(err);
    
    // Initialize object
    object obj = {res_x/20, res_y/20, {0,0}};

    // Loop
    bool exit = false;
    while(!exit) {
        
        // Clear the buffer
        err = edl_clear_screen(&screen,
                               0xFF000000); // Fill screen with black
        exit_if_error(err);

        // Draw object (red rectangle)
        for (edl_u32 j=0; j<obj.height; j++) {
            for (edl_u32 i=0; i<obj.width; i++) {
                screen.buffer[(obj.position[0]+i) + (obj.position[1]+j) * res_x] = 0x88FF0000;
            }
        }

        // Show screen
        err = edl_show_screen(&screen);
        exit_if_error(err);
        
        // New position for object
        obj.position[0] += obj.width;
        obj.position[1] += obj.height;
        
        // Check exit criteria
        if (obj.position[0] >= res_x-obj.width/2) exit = true;
        if (obj.position[1] >= res_y-obj.height/2) exit = true;
    }
    
    // Free memory at the end of the execution
    err = edl_dalloc_screen(&screen);
    exit_if_error(err);
    
    return 0;
}
