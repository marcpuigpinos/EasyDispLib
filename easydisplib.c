#include "easydisplib.h"

/** CONSTANTS **/
static const int DIGITS = 4;

/** EDL_COLOR PROCEDURES **/

// From hexa to rgba
int edl_from_hexa_to_rgba(const edl_u32 color,
                          unsigned char *r,
                          unsigned char *g,
                          unsigned char *b,
                          unsigned char *a)
{

    if (!r || !g || !b || !a)
        return EDL_FAILURE;
    
    *a = (color >> 24) & 0xFF;            
    *r = (color >> 16) & 0xFF;
    *g = (color >> 8)  & 0xFF;
    *b = (color)       & 0xFF;

    return EDL_SUCCESS;
        
}

// From rgba to hexa
int edl_from_rgba_to_hexa(const unsigned char r,
                          const unsigned char g,
                          const unsigned char b,
                          const unsigned char a,
                          edl_u32 *color)
{

    if (!color)
        return EDL_FAILURE;

    // Use bitewise OR to combine the values
    *color = ((edl_u32)a << 24) |
             ((edl_u32)r << 16) |
             ((edl_u32)g << 8)  |
             ((edl_u32)b);

    return EDL_SUCCESS;
    
}

// Mix color
int edl_mix_color(const edl_u32 cf, // Color foreground
                  const edl_u32 cb, // Color background
                  edl_u32 *cp)      // Color pixel
{
    
    if (!cp)
        return EDL_FAILURE;

    int err = 0;
    
    // Mix colors by blending.
    /**
       Alpha blending: https://en.wikipedia.org/wiki/Alpha_compositing
       ap = af + ab * (1 - af) (0-1)
       ap = af + (ab * (255 - af) / 255) (0-255)
       cp = (cf * af + cb * ab * (1 - af)) / ap (0-1)
       cp = (cf * af + cb * ab * (255 - af)/255) / ap (0-255)
    **/ 
    unsigned char af = 0, rf = 0, gf = 0, bf = 0;
    unsigned char ab = 0, rb = 0, gb = 0, bb = 0;

    err = edl_from_hexa_to_rgba(cf, &rf, &gf, &bf, &af);
    if (err == EDL_FAILURE)
        return EDL_FAILURE;
    
    err = edl_from_hexa_to_rgba(cb, &rb, &gb, &bb, &ab);
    if (err == EDL_FAILURE)
        return EDL_FAILURE;

    // Compute alfa blending
    unsigned char ap = af + (ab * (255 - af) / 255);

    // Compute colors, r, g, b, blending.
    unsigned char rp = (rf * af + rb * ab * (255 - af)/255) / ap; // r
    unsigned char gp = (gf * af + gb * ab * (255 - af)/255) / ap; // g
    unsigned char bp = (bf * af + bb * ab * (255 - af)/255) / ap; // b

    // Convert to hexa color.
    err = edl_from_rgba_to_hexa(rp, gp, bp, ap, cp);

    if (err == EDL_FAILURE)
        return EDL_FAILURE;

    return EDL_SUCCESS;
    
}

/** END EDL_COLOR PROCEDURES **/

/** EDL_SCREEN PROCEDURES **/

// Initialize EDL_SCREEN
int edl_init_screen(EDL_SCREEN *screen,
                    const edl_u32 res_x,
                    const edl_u32 res_y,
                    edl_u32 color)
{

    // Check screen is not null
    if (screen == NULL) {
        return EDL_FAILURE;
    }
    
    // Create the screen 
    screen->res_x = res_x;
    screen->res_y = res_y;
    free(screen->buffer);
    screen->buffer = NULL;
    screen->buffer = (edl_u32 *)malloc(res_x * res_y * sizeof(edl_u32));
    for (edl_u32 j = 0; j<res_y; j++) {
        for (edl_u32 i = 0; i<res_x; i++) {
            screen->buffer[i+j*res_x] = color;
        }
    }    
    return EDL_SUCCESS;

}

// Deallocate EDL_SCREEN
int edl_dalloc_screen(EDL_SCREEN *screen)
{

    // Check screen is not null
    if (screen == NULL) {
        return EDL_FAILURE;
    }

    // Free memory
    free(screen->buffer);
    screen->buffer = NULL;
    screen->res_x = -1;
    screen->res_y = -1;
    return EDL_SUCCESS;

}  

// Show EDL_SCREEN
int edl_show_screen(const EDL_SCREEN *screen)
{

    // Check screen is not null
    if (screen == NULL) {
        return EDL_FAILURE;
    }
    
    static size_t img_count = 0; // static local var counting the images  
    char filename[255];    // buffer for filename
    char format[20];      // buffer for format string
    
    // build the format string
    snprintf(format, sizeof(format), "output_%%0%dd.pam", DIGITS);
    
    // open the filename
    FILE *fp;
    snprintf(filename, sizeof(format), format, img_count);
    fp = fopen(filename, "wb");
    
    // Write the header
    fprintf(fp, "P7\nWIDTH %d\n"
                "HEIGHT %d\n"
                "DEPTH 4\n"
                "MAXVAL 255\n"
                "TUPLTYPE RGB_ALPHA\n"
                "ENDHDR\n",
                 screen->res_x, screen->res_y
    );
    
    for (edl_u32 j=0; j<screen->res_y; j++) {
        for (edl_u32 i=0; i<screen->res_x; i++) {
            edl_u32 color = screen->buffer[i+j*screen->res_x];
            unsigned char r = 0,
                          g = 0,
                          b = 0,
                          a = 0;
            int err = edl_from_hexa_to_rgba(color, &r, &g, &b, &a);
            if (err == EDL_FAILURE) {
                // exit with error
                fclose(fp);
                fp = NULL;
                return EDL_FAILURE;
            }
            fputc(r, fp);
            fputc(g, fp);
            fputc(b, fp);
            fputc(a, fp);            
        }
    }    
    img_count += 1;
    fclose(fp);
    fp = NULL;
    
    return EDL_SUCCESS;
    
}

int edl_clear_screen(EDL_SCREEN *screen,
                     edl_u32 color)
{

    // Check screen is not null
    if (screen == NULL) {
        return EDL_FAILURE;
    }
       
    // Clear the screen
    for (edl_u32 j=0; j<screen->res_y; j++) {
        for (edl_u32 i=0; i<screen->res_x; i++) {
            screen->buffer[i+j*screen->res_x] = color;
        }
    }

    return EDL_SUCCESS;
    
}

/** END EDL_SCREEN PROCEDURES **/


/** EDL_SPRITE PROCEDURES **/

// Initialize sprite
int edl_init_sprite(EDL_SPRITE *sprite)
{

    // Check if sprite is not allocated
    if (sprite == NULL) {
        return EDL_FAILURE;
    }

    // Initialize sprite
    sprite->res_x = -1;
    sprite->res_y = -1;
    free(sprite->img);
    sprite->img = NULL;
    
    return EDL_SUCCESS;
    
}

// Deallocate sprite
int edl_dalloc_sprite(EDL_SPRITE *sprite)
{

    // Check if sprite is not allocated
    if (sprite == NULL) {
        return EDL_FAILURE;
    }

    // Dalloc sprite
    sprite->res_x = -1;
    sprite->res_y = -1;
    free(sprite->img);
    sprite->img = NULL;

    return EDL_SUCCESS;
    
}

int edl_load_sprite(EDL_SPRITE *sprite,
                    char *filepath)
{

    // Check if sprite is not allocated
    if (sprite == NULL) {
        return EXIT_FAILURE;
    }

    // Open the file
    FILE *fp = fopen(filepath, "rb");

    // Close the file and free nullify the pointer
    fclose(fp);
    fp = NULL;
    return EDL_SUCCESS;
    
}

/** END EDL_SPRITE PROCEDURES **/
