#include "easydisplib.h"

// Initialize EDL_SCREEN
int edl_init_screen(EDL_SCREEN *screen, const u32 res_x, const u32 res_y, u32 color) {

    // Check screen is not null
    if (screen == NULL) {
        return EDL_FAILURE;
    }
    
    // Create the screen 
    screen->res_x = res_x;
    screen->res_y = res_y;
    screen->buffer = (u32 *)malloc(res_x * res_y * sizeof(u32));
    for (u32 j = 0; j<res_y; j++) {
        for (u32 i = 0; i<res_x; i++) {
            screen->buffer[i+j*res_x] = color;
        }
    }    
    return EDL_SUCCESS;

}

// Deallocate EDL_SCREEN
int edl_dalloc_screen(EDL_SCREEN *screen) {

    // Check screen is not null
    if (screen == NULL) {
        return EDL_FAILURE;
    }

    // Free memory
    free(screen->buffer);
    screen->res_x = 0;
    screen->res_y = 0;
    return EDL_SUCCESS;

}  

// Show EDL_SCREEN
int edl_show_screen(const EDL_SCREEN *screen) {

    // Check screen is not null
    if (screen == NULL) {
        return EDL_FAILURE;
    }
   
    static size_t img_count = 0; // static local var counting the images  
    char filename[255];    // buffer for filename
    char format[20];      // buffer for format string
    
    // build the format string
    snprintf(format, sizeof(format), "output_%%0%dd.ppm", DIGITS);
    
    // open the filename
    FILE *fp;
    snprintf(filename, sizeof(format), format, img_count);
    fp = fopen(filename, "wb");
    
    // Write the header
    fprintf(fp, "P6\n%d %d\n255\n", screen->res_x, screen->res_y);
    
    for (u32 j=0; j<screen->res_y; j++) {
        for (u32 i=0; i<screen->res_x; i++) {
            u32 color = screen->buffer[i+j*screen->res_x];
            unsigned char r = (color >> 16) & 0xFF;
            unsigned char g = (color >> 8)  & 0xFF;
            unsigned char b = (color)       & 0xFF;
            fputc(r, fp);
            fputc(g, fp);
            fputc(b, fp);
        }
    }    
    img_count += 1;
    fclose(fp);
    fp = NULL;
    return EDL_SUCCESS;
    
}

int edl_clear_screen(EDL_SCREEN *screen, u32 color) {

    // Check screen is not null
    if (screen == NULL) {
        return EDL_FAILURE;
    }
       
    // Clear the screen
    for (u32 j=0; j<screen->res_y; j++) {
        for (u32 i=0; i<screen->res_x; i++) {
            screen->buffer[i+j*screen->res_x] = color;
        }
    }

    return EDL_SUCCESS;
    
}
