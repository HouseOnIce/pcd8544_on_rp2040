# pcd8544_on_rp2040
This is a PCD8544 LCD driver library suitable for the RP2040 microcontroller.

***

# Usage method

## Initialization

** Header file and pin definitions **
```
#include "pcd8544.h"

// Define your SPI and GPIO pins here
#define SPI_PORT spi0
#define PIN_SCK  2
#define PIN_MOSI 3
#define PIN_CS   5
#define PIN_DC   4
#define PIN_RST  6
```

** Initialize in main() **
```
    pcd8544_t lcd;
    pcd8544_pins_t pins = {
        .spi_port = SPI_PORT,
        .sck_pin = PIN_SCK,
        .mosi_pin = PIN_MOSI,
        .cs_pin = PIN_CS,
        .dc_pin = PIN_DC,
        .rst_pin = PIN_RST
    };

    pcd8544_init(&lcd, pins);
```

## Usage of the drawing function

`pcd8544_clear(&lcd)`:Clear the screen.

`pcd8544_display_update(&lcd)`:Update buffer.

`pcd8544_draw_pixel(&lcd, x, y, color)`:Draw pixels. Where `x` is the horizontal coordinate, `y` is the vertical coordinate, and `color` is the display color (0 for white, 1 for black, and so on).

`pcd8544_draw_line(&lcd, x1, y1, x2, y2, color)`:Draw a line. `x1` is the x-coordinate of the starting point, `y1` is the y-coordinate of the starting point, `x2` is the x-coordinate of the endpoint, `y2` is the y-coordinate of the endpoint, and `color` is the display color.

`pcd8544_draw_rectangle(&lcd, x, y, w, h, color, fill)`:Draw a rectangle. `x` is the starting horizontal coordinate, `y` is the starting vertical coordinate, `w` is the width of the rectangle, `h` is the height of the rectangle, `color` is the display color, and `fill` indicates whether to fill it.


`pcd8544_draw_char(&lcd, x, y, c, color)`:Display a single ASCII character. `x` is the x-coordinate, `y` is the y-coordinate, `c` is the character, and `color` is the display color.

`pcd8544_draw_string(&lcd, x, y, str, color)`:Display ASCII string. `x` is the x-coordinate, `y` is the y-coordinate, `str` is the string, and `color` is the display color.

***

# Compilation Notes
Please be sure to add `pcd8544.c` to the `CMakeLists.txt` file and add the `hardware_spi` dependency in the `CMakeLists.txt`.
