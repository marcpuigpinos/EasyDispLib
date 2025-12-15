#include <stdbool.h>
#include <stdlib.h>

#include "../easydisplib.h"


#define FPS 10
#define FRAME_TIME (1000000000 / FPS)

typedef struct {
    u32 x;
    u32 y;
} object;

void exit_if_error(int err) {
    if (err == 0) return;
    printf("Program abnormally exited with status: %d\n", err);
    exit(err);
}


int main() {

    // Error
    int err;

    // Declaration of a screen 50x50 characters
    EDL_SCREEN screen;
    const u32 res_x = 800;
    const u32 res_y = 600;

    // Initialize screen with black
    err = edl_init_screen(&screen, res_x, res_y, 0xFF000000);
    exit_if_error(err);
    
    // Initialize object
    object obj = {0,0};

    // Loop
    bool exit = false;
    while(!exit) {
        
        // Check exit criteria
        if (obj.x>res_x-1) exit = true;
        if (obj.y>res_y-1) exit = true;

        
        // Update the buffer
        err = edl_clear_screen(&screen, 0xFF000000); // Fill screen with black
        exit_if_error(err);
        screen.buffer[obj.x + obj.y * screen.res_x] = 0xFFFF0000;

        // Show screen
        err = edl_show_screen(&screen);
        exit_if_error(err);
        
        // New position for object
        obj.x += 1; obj.y += 1;
    }
    
    // Free memory at the end of the execution
    err = edl_dalloc_screen(&screen);
    exit_if_error(err);
    
    return 0;
}
