#ifndef EASYDISPLIB_H
#define EASYDISPLIB_H

#include <stdio.h>
#include <stdlib.h>

#define EDL_SUCCESS 0
#define EDL_FAILURE 1

#define u32 unsigned int

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

// Screen structure
typedef struct {
    u32 res_x;
    u32 res_y;
    u32 *buffer;
} EDL_SCREEN;

// Initialize EDL_SCREEN
int edl_init_screen(EDL_SCREEN *screen, const u32 res_x, const u32 res_y, u32 ascii_code) {
    screen->res_x = res_x;
    screen->res_y = res_y;
    screen->buffer = (u32 *)malloc(res_x * res_y * sizeof(u32));
    for (u32 i = 0; i<res_x; i++) {
        for (u32 j = 0; j<res_y; j++) {
            screen->buffer[i+j*res_x] = ascii_code;
        }
    }    
    return EDL_SUCCESS;
}

// Deallocate EDL_SCREEN
int edl_dalloc_screen(EDL_SCREEN *screen) {
    free(screen->buffer);
    screen->res_x = 0;
    screen->res_y = 0;
    return EDL_SUCCESS;
}

// Update EDL_SCREEN
int edl_update_screen(EDL_SCREEN *screen, const u32 ascii_code) {
    for (u32 i=0; i<screen->res_x; i++) {
        for (u32 j=0; j <screen->res_y; j++) {
            screen->buffer[i+j*screen->res_x] = ascii_code;
        }
    }
    return EDL_SUCCESS;
}
    

// Show EDL_SCREEN
int edl_show_screen(const EDL_SCREEN *screen) {
    for (u32 i=0; i<screen->res_x; i++) {
        for (u32 j=0; j<screen->res_y; j++) {
            printf("%c ", ASCII_TABLE[screen->buffer[i+j*screen->res_x]]);
        }
        printf("\n");
    }    
    return EDL_SUCCESS;
}

// Clean EDL_SCREEN:
//- \033 is the escape character (octal for ASCII 27)
//- [2J clears the entire screen
//- [H moves the cursor to the home position (row 1, column 1)
int edl_clean_screen() {
    // Clear screen and move cursor to top-left
    printf("\033[2J\033[H");
    return EDL_SUCCESS;
}

#endif
