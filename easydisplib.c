#include "easydisplib.h"

/** CONSTANTS **/
static const int DIGITS = 4;
static const float EPS = 0.001;

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
    screen->res_x = 0;
    screen->res_y = 0;
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

int edl_write_sprite_on_buffer(EDL_SCREEN *screen,
                               const EDL_SPRITE *sprite,
                               const edl_u32 pos_x,
                               const edl_u32 pos_y)
{

    // Check if screen is not allocated
    if (screen == NULL)
        return EDL_FAILURE;

    // Check if sprite is not allocated
    if (sprite == NULL)
        return EDL_FAILURE;
    
    // If position is out of screen, nothing to draw.
    if (pos_x >= screen->res_x || pos_y >= screen->res_y)
        return EDL_SUCCESS;
    
    // Write the sprite on the buffer.
    for (edl_u32 j=0; j<sprite->height; j++) {
        for (edl_u32 i=0; i<sprite->width; i++) {
            // Not draw pixels out of screen
            if (pos_x+i >= screen->res_x || pos_y + j >= screen->res_y)
                continue;
            // rgba color components forground
            edl_u32 cf = sprite->img[i+j*sprite->width];
            //rgba color components background
            edl_u32 k = (pos_x+i) + (pos_y+j) * screen->res_x;
            edl_u32 cb = screen->buffer[k];
            // Mix the colors
            edl_u32 cm = 0;
            int err = edl_mix_color(cf, cb, &cm);
            if (err == EDL_FAILURE)
                return EDL_FAILURE;
            screen->buffer[k] = cm;
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
    sprite->width = 0;
    sprite->height = 0;
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
    sprite->width = 0;
    sprite->height = 0;
    free(sprite->img);
    sprite->img = NULL;

    return EDL_SUCCESS;
    
}

// Line sprite
int edl_line_sprite(EDL_SPRITE *sprite,
                    const EDL_VEC2 p1,
                    const EDL_VEC2 p2,
                    const edl_u32 color)
{
    // Check if sprite is not allocated
    if (sprite == NULL) {
        return EXIT_FAILURE;
    }
    // Compute the width and the height of the sprite.
    // Compute the bounding box
    EDL_VEC2 pmin = {UINT_MAX, UINT_MAX};
    EDL_VEC2 pmax = {0, 0};
    // Min x
    if (p1.x < pmin.x) pmin.x = p1.x;
    if (p2.x < pmin.x) pmin.x = p2.x;
    // Min y
    if (p1.y < pmin.y) pmin.y = p1.y;
    if (p2.y < pmin.y) pmin.y = p2.y;
    // Max x
    if (p1.x > pmax.x) pmax.x = p1.x;
    if (p2.x > pmax.x) pmax.x = p2.x;
    // Max y
    if (p1.y > pmax.y) pmax.y = p1.y;
    if (p2.y > pmax.y) pmax.y = p2.y;
    // Compute width and height of the sprite. 3x3 minimum
    sprite->width = pmax.x - pmin.x;
    if (sprite->width < 3) sprite->width = 3;
    sprite->height = pmax.y - pmin.y;
    if (sprite->height < 3) sprite->height = 3;    
    // Free sprite
    free(sprite->img);
    
    // Allocate sprite
    sprite->img = (edl_u32 *)malloc(sprite->width * sprite->height * sizeof(edl_u32));
    // Assign full transparency for the whole sprite
    for (edl_u32 j = 0; j < sprite->height; j++) {
        for (edl_u32 i = 0; i < sprite->width; i++) {
            sprite->img[i + j*sprite->width] = 0x00000000;
        }
    }

    // Draw the line starting from p1 to p2.
    // y - y1 = m * (x - x1)
    // m = (y2 - y1) / (x2-x1)
    int32_t dx = p2.x - p1.x;
    int32_t dy = p2.y - p1.y;
    float m = (float)dy / (float)dx;
    for (edl_u32 i = pmin.x; i<=pmax.x; i++) {
        edl_u32 j = (edl_u32)((float)p1.y +  m * (float)(i - p1.x));
        sprite->img[i + j*sprite->width] = color;
    }
    
    return EXIT_SUCCESS;
    
}
                            

// Triangle sprite
int edl_triangle_sprite(EDL_SPRITE *sprite,
                        const EDL_VEC2 v1,
                        const EDL_VEC2 v2,
                        const EDL_VEC2 v3,
                        const edl_u32 color)
{
    // Check if sprite is not allocated
    if (sprite == NULL) {
        return EXIT_FAILURE;
    }

    // Compute bounding box
    EDL_VEC2 pmin = {UINT_MAX, UINT_MAX};
    EDL_VEC2 pmax = {0, 0};
    
    // Min
    if (v1.x < pmin.x) pmin.x = v1.x;
    if (v2.x < pmin.x) pmin.x = v2.x;
    if (v3.x < pmin.x) pmin.x = v3.x;     
    if (v1.y < pmin.y) pmin.y = v1.y;
    if (v2.y < pmin.y) pmin.y = v2.y;
    if (v3.y < pmin.y) pmin.y = v3.y;     
    
    // Max
    if (v1.x > pmax.x) pmax.x = v1.x;
    if (v2.x > pmax.x) pmax.x = v2.x;
    if (v3.x > pmax.x) pmax.x = v3.x;     
    if (v1.y > pmax.y) pmax.y = v1.y;
    if (v2.y > pmax.y) pmax.y = v2.y;
    if (v3.y > pmax.y) pmax.y = v3.y;
    
    // Compute width and height. 3x3 minimum
    sprite->width = pmax.x - pmin.x;
    if (sprite->width < 3) sprite->width = 3;
    sprite->height = pmax.y - pmin.y;
    if (sprite->height < 3) sprite->height = 3;        
    
    // Free old and allocate new
    free(sprite->img);
    sprite->img = (edl_u32 *)malloc(sprite->width * sprite->height * sizeof(edl_u32));
    
    // Check for allocation failure
    if (sprite->img == NULL) return EXIT_FAILURE; 

    // Cast point coordinates to float
    float x1 = (float)v1.x; float y1 = (float)v1.y;
    float x2 = (float)v2.x; float y2 = (float)v2.y;
    float x3 = (float)v3.x; float y3 = (float)v3.y;

    // Barycentric coordinates: https://en.wikipedia.org/wiki/Barycentric_coordinate_system
    // Compute denominator once
    float denominator = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);

    // Avoid division by zero. This can happen if vertex are coincident.
    if (fabs(denominator) < EPS) return EXIT_FAILURE; 

    float inv_denominator = 1.0f / denominator;

    // Draw the triangle
    for (edl_u32 j = 0; j < sprite->height; j++) {
        // Global Y coordinate of the current pixel
        float py = (float)(pmin.y + j); 

        for (edl_u32 i = 0; i < sprite->width; i++) {
            // Global X coordinate of the current pixel
            float px = (float)(pmin.x + i);

            // Barycentric Coordinates
            float w1 = ((y2 - y3) * (px - x3) + (x3 - x2) * (py - y3)) * inv_denominator;
            float w2 = ((y3 - y1) * (px - x3) + (x1 - x3) * (py - y3)) * inv_denominator;
            float w3 = 1.0f - w1 - w2;

            // Check if inside
            // No need to check if wi <= 1.0, because wi belongs [0,1] and the property w1 + w2 + w3 = 1
            // So if w1 = 1.2, w2 = 0.5, as w3 is computed as 1 - w1 - w2, w3 = 1 - 1.2 - 0.5 = -0.7
            bool inside = (w1 >= -EPS && w2 >= -EPS && w3 >= -EPS);

            if (inside) {
                sprite->img[i + j * sprite->width] = color;
            } else {
                sprite->img[i + j * sprite->width] = 0x00000000;
            }
        }
    }
        
    return EXIT_SUCCESS;
}

// Square sprite
int edl_square_sprite(EDL_SPRITE *sprite,
                      const edl_u32 width,
                      const edl_u32 height,
                      const edl_u32 color)
{

    // Check if sprite is not allocated
    if (sprite == NULL) {
        return EXIT_FAILURE;
    }

    // Check if width and height are ok
    if (width <= 0 || height <=0) {
        return EXIT_FAILURE;
    }
    
    // Free sprite->img pointer
    free(sprite->img);

    // Create square sprite
    sprite->width = width;
    sprite->height = height;
    sprite->img = (edl_u32 *)malloc(width * height * sizeof(edl_u32));

    for (edl_u32 j = 0; j < height; j++) {
        for (edl_u32 i = 0; i < width; i++) {
            sprite->img[i + j*width] = color;
        }
    }

    return EXIT_SUCCESS;
    
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
