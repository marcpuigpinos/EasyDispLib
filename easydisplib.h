#ifndef EASYDISPLIB_H
#define EASYDISPLIB_H

#define EDL_SUCCESS 0
#define EDL_FAILURE 1

// Buffer structure
typdef struct {
    int *buffer;
} EDL_BUFFER;

// Initialize EDL_BUFFER
int edl_init_buffer(EDL_BUFFER* buffer, const int res_x, const int res_y) {
    buffer->buffer = (*int) malloc(res_x * res_y * sizeof(int));
    for (int i=0; i<res_x; i++) {
        for (int j=0; j<res_y; j++) {
            buffer->buffer[i+j*res_x] = 0;
        }
    }
    return EDL_SUCCESS;
}

// Screen structure
typedef struct {
    int res_x;
    int res_y;
    EDL_BUFFER buffer;
} EDL_SCREEN;

// Initialize EDL_SCREEN
int edl_init_screen(EDL_SCREEN *screen, const int res_x, const int res_y) {
    screen->res_x = res_x;
    screen->res_y = res_y;
    int err = edl_init_buffer(&screen->buffer, res_x, res_y);
    return EDL_SUCCESS;
}

// Return EDL_SCREEN resolution
int edl_get_resolution(const EDL_SCREEN *screen, int *res_x, int *res_y) {
    *res_x = screen->res_x;
    *res_y = screen->res_y;
    return EDL_SUCCESS;
}

#endif
