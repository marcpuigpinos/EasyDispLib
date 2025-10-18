#ifndef EASYDISPLIB_H
#define EASYDISPLIB_H

#include <stdlib.h>

#define EDL_SUCCESS 0
#define EDL_FAILURE 1

// ASCII table: https://www.ascii-code.com/
const char ASCII_TABLE[128] = {
  '\x00',   '\x01',   '\x02',   '\x03',   '\x04',   '\x05',   '\x06',   '\x07', \
  '\x08',   '\x09',   '\x0A',   '\x0B',   '\x0C',   '\x0D',   '\x0E',   '\x0F', \
  '\x10',   '\x11',   '\x12',   '\x13',   '\x14',   '\x15',   '\x16',   '\x17', \
  '\x18',   '\x19',   '\x1A',   '\x1B',   '\x1C',   '\x1D',   '\x1E',   '\x1F', \
  ' ',   '!',   '\x22',   '#',   '$',   '%',   '&',   '\x27', \
  '(',   ')',   '*',   '+',   ',',   '-',   '.',   '/', \
  '0',   '1',   '2',   '3',   '4',   '5',   '6',   '7', \
  '8',   '9',   ':',   ';',   '<',   '=',   '>',   '?', \
  '@',   'A',   'B',   'C',   'D',   'E',   'F',   'G', \
  'H',   'I',   'J',   'K',   'L',   'M',   'N',   'O', \
  'P',   'Q',   'R',   'S',   'T',   'U',   'V',   'W', \
  'X',   'Y',   'Z',   '[',   '\x5C',   ']',   '^',   '_', \
  '`',   'a',   'b',   'c',   'd',   'e',   'f',   'g', \
  'h',   'i',   'j',   'k',   'l',   'm',   'n',   'o', \
  'p',   'q',   'r',   's',   't',   'u',   'v',   'w', \
  'x',   'y',   'z',   '{',   '|',   '}',   '~',   '\x7F',
};


// Buffer structure
typedef struct {
    int *buffer;
    int res_x;
    int res_y;
} EDL_BUFFER;

// Initialize EDL_BUFFER
int edl_init_buffer(EDL_BUFFER *buffer, const int res_x, const int res_y) {
    buffer->res_x = res_x; buffer->res_y = res_y;
    buffer->buffer = (int *)malloc(res_x * res_y * sizeof(int));
    for (int i = 0; i<res_x; i++) {
        for (int j = 0; j<res_y; j++) {
            buffer->buffer[i+j*res_x] = 32;
        }
    }
    return EDL_SUCCESS;
}

// Show EDL_BUFFER
int edl_show_buffer(const EDL_BUFFER *buffer) {
    for (int i=0; i<buffer->res_x; i++) {
        for (int j=0; j<buffer->res_y; j++) {
            printf("%c ", ASCII_TABLE[buffer->buffer[i+j*buffer->res_x]]);
        }
        printf("\n");
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
    if (err == EDL_FAILURE)
        return err;
    return EDL_SUCCESS;
}

// Return EDL_SCREEN resolution
int edl_get_resolution(const EDL_SCREEN *screen, int *res_x, int *res_y) {
    *res_x = screen->res_x;
    *res_y = screen->res_y;
    return EDL_SUCCESS;
}

// Show EDL_SCREEN
int edl_show_screen(const EDL_SCREEN *screen) {
    int err = edl_show_buffer(&screen->buffer);
    if (err == EDL_FAILURE) return EDL_FAILURE;
    return EDL_SUCCESS;
}

#endif
