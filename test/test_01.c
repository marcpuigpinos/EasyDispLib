#include <time.h>
#include <unistd.h>

#include "../easydisplib.h"


#define FPS 10
#define FRAME_TIME (1000000000 / FPS)

typedef struct {
    u32 x;
    u32 y;
} object;


int main() {

    // Error
    int err;

    // Vars for taking care of time
    struct timespec start, end;
    long elapsed_ns;
    
    // Declaration of a screen 50x50 characters
    EDL_SCREEN screen;
    const u32 res_x = 50;
    const u32 res_y = 50;

    // Initialize screen with empty space, ascii-code: 32
    err = edl_init_screen(&screen, res_x, res_y, 32);

    // Initialize object
    object obj = {0,0};

    // Loop
    bool exit = false;
    while(!exit) {
        //Start counting clocktime
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        // Check exit criteria
        if (obj.x>res_x-1) exit = true;
        if (obj.y>res_y-1) exit = true;

        // Clear screen
        err = edl_clean_screen();

        // Update the buffer
        err = edl_update_screen(&screen, 32); // Fill screen with empty space, ascii-code: 32
        screen.buffer[obj.x + obj.y * screen.res_x] = 64;

        // Show screen
        err = edl_show_screen(&screen);
        
        // New position for object
        obj.x += 1; obj.y += 1;
        
        // End counting the time
        clock_gettime(CLOCK_MONOTONIC, &end);
        // Time taken for frame
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000L +
                     (end.tv_nsec - start.tv_nsec);
        long remaining = FRAME_TIME - elapsed_ns;
        if (remaining > 0) {
            struct timespec sleep_time = {
                .tv_sec = remaining / 1000000000,
                .tv_nsec = remaining % 1000000000
            };
            nanosleep(&sleep_time, NULL);
        }
    }
    
    // Free memory at the end of the execution
    err = edl_dalloc_screen(&screen);
    
    return 0;
}
