#include <stdio.h>

#include "../easydisplib.h"

int main() {
    printf("Hello World!\n");
    EDL_SCREEN screen;
    int err = 0;
    err = edl_init_screen(&screen, 50, 50);
    if (err != 0)
        return -1;
    int res_x = 0;
    int res_y = 0;
    err = edl_get_resolution(&screen, &res_x, &res_y);
    if (err != 0)
        return -1;
    printf("The resolution is %d x %d chars.\n", res_x, res_y);
    err = edl_show_screen(&screen);
    return 0;
}
