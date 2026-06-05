# EasyDispLib

A lightweight C library for drawing 2D sprites on a pixel framebuffer and exporting snapshots in PAM (P7) format. Provides simple geometric primitives, alpha blending, and PAM sprite loading — all with zero external dependencies beyond the C standard library.

## Features

- **Framebuffer screen** — allocate, clear, and manage an RGBA pixel buffer
- **2D primitives** — lines, filled triangles, filled rectangles, filled circles (as sprites)
- **Sprite loading** — load RGBA sprites from PAM (P7) image files
- **Alpha blending** — compose sprites onto the screen with proper alpha compositing
- **PAM snapshots** — export the screen buffer to a PPM-like PAM file at any time
- **Clipping** — sprites are automatically clipped at screen boundaries
- **No external dependencies** — only the C standard library (`stdio`, `stdlib`, `math`, etc.)

## Project Structure

```
├── easydisplib.h        # Public API header
├── easydisplib.c        # Library implementation
├── test/
│   └── test_pam.c       # Demo / test program
├── sprite/
│   ├── sprite.pam       # Example sprite (PAM format)
│   └── sprite.xcf       # Sprite source (GIMP)
├── makefile
├── LICENSE
├── AGENTS.md
└── README.md
```

## Build & Run

```sh
make          # Compile the test program
make run_pam  # Run the test/demo
make clean    # Remove build artifacts
```

The build uses `clang` with `-g -Wall -Wextra` and outputs to `build/test_pam`.

## API Overview

### Data Structures

| Type | Fields | Description |
|------|--------|-------------|
| `EDL_SCREEN` | `res_x`, `res_y`, `buffer` | Pixel framebuffer (32-bit RGBA) |
| `EDL_SPRITE` | `width`, `height`, `img` | Sprite image data |
| `EDL_VEC2` | `x`, `y` | 2D unsigned integer vector |

All types are `unsigned int` based — coordinates and dimensions are non-negative.

### Color Functions

```c
int edl_from_hexa_to_rgba(edl_u32 color, unsigned char *r, unsigned char *g,
                          unsigned char *b, unsigned char *a);
int edl_from_rgba_to_hexa(unsigned char r, unsigned char g, unsigned char b,
                          unsigned char a, edl_u32 *color);
int edl_mix_color(edl_u32 cf, edl_u32 cb, edl_u32 *cp);
```

- `edl_from_hexa_to_rgba` — unpack a packed 32-bit `A|R|G|B` color into byte components
- `edl_from_rgba_to_hexa` — pack RGBA byte components into a 32-bit value
- `edl_mix_color` — alpha-blend foreground color over background color

### Screen Functions

```c
int edl_init_screen(EDL_SCREEN *screen, EDL_VEC2 resolution, edl_u32 color);
int edl_dalloc_screen(EDL_SCREEN *screen);
int edl_clear_screen(EDL_SCREEN *screen, edl_u32 color);
int edl_take_snapshot(const EDL_SCREEN *screen);
int edl_write_sprite_on_buffer(EDL_SCREEN *screen, const EDL_SPRITE *sprite,
                               EDL_VEC2 position);
```

- `edl_init_screen` — allocate and fill a framebuffer of given dimensions
- `edl_dalloc_screen` — free the framebuffer memory
- `edl_clear_screen` — fill the entire buffer with a solid color
- `edl_take_snapshot` — write the current buffer to `output_XXXX.pam` (auto-incremented)
- `edl_write_sprite_on_buffer` — draw a sprite onto the screen at `(x, y)` with alpha blending and edge clipping

### Sprite Functions

```c
int edl_init_sprite(EDL_SPRITE *sprite);
int edl_dalloc_sprite(EDL_SPRITE *sprite);
int edl_line_sprite(EDL_SPRITE *sprite, EDL_VEC2 p1, EDL_VEC2 p2, edl_u32 color);
int edl_triangle_sprite(EDL_SPRITE *sprite, EDL_VEC2 v1, EDL_VEC2 v2, EDL_VEC2 v3,
                        edl_u32 color);
int edl_square_sprite(EDL_SPRITE *sprite, edl_u32 width, edl_u32 height,
                      edl_u32 color);
int edl_circle_sprite(EDL_SPRITE *sprite, edl_u32 radius, edl_u32 color);
int edl_load_sprite(EDL_SPRITE *sprite, char filepath[]);
```

- `edl_init_sprite` — initialise a sprite to empty (width/height = 0, img = NULL)
- `edl_dalloc_sprite` — free the sprite's image memory and reset dimensions
- `edl_line_sprite` — create a sprite containing a single line from `p1` to `p2`
- `edl_triangle_sprite` — create a filled triangle sprite using barycentric coordinates
- `edl_square_sprite` — create a filled rectangle sprite
- `edl_circle_sprite` — create a filled circle sprite
- `edl_load_sprite` — load a sprite from a PAM (P7) file (DEPTH 4, MAXVAL 255, TUPLTYPE RGB_ALPHA)

## Return Values

All public functions return `int`:

| Constant | Value | Meaning |
|----------|-------|---------|
| `EDL_SUCCESS` | `0` | Operation succeeded |
| `EDL_FAILURE` | `1` | Operation failed (null pointer, allocation error, file I/O error, etc.) |

## Pixel Format

- Packed 32-bit RGBA (`uint32_t`)
- Byte order in memory: `A | R | G | B` (alpha in the most significant byte)
- Supported PAM sprites must be: P7 binary, DEPTH 4, MAXVAL 255, TUPLTYPE RGB_ALPHA

## Output Format

Snapshots are saved as **PAM (P7)** files:

```
P7
WIDTH <w>
HEIGHT <h>
DEPTH 4
MAXVAL 255
TUPLTYPE RGB_ALPHA
ENDHDR
<raw RGBA pixel data>
```

Files are named sequentially: `output_0000.pam`, `output_0001.pam`, etc.

## Example Usage

```c
#include "easydisplib.h"

int main() {
    EDL_SCREEN screen;
    EDL_VEC2 res = {800, 600};

    // Create a screen filled with green
    edl_init_screen(&screen, res, 0xFF008800);

    // Create a red square sprite
    EDL_SPRITE square;
    edl_init_sprite(&square);
    edl_square_sprite(&square, 50, 50, 0x88FF0000);

    // Draw the sprite onto the screen
    EDL_VEC2 pos = {100, 200};
    edl_write_sprite_on_buffer(&screen, &square, pos);

    // Save a snapshot
    edl_take_snapshot(&screen);

    // Clean up
    edl_dalloc_sprite(&square);
    edl_dalloc_screen(&screen);
    return 0;
}
```

## License

MIT License — see [LICENSE](LICENSE).  
Copyright (c) 2026 Marc Puigpinos.
