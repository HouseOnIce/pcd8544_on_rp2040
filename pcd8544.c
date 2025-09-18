#include "pcd8544.h"
#include "pcd8544_font.h" // Custom font file
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

// Internal function to send byte via SPI
static void pcd8544_spi_send_byte(pcd8544_t *lcd, uint8_t data) {
    spi_write_blocking(lcd->pins.spi_port, &data, 1);
}

// Internal function to send command
void pcd8544_command(pcd8544_t *lcd, uint8_t command) {
    gpio_put(lcd->pins.dc_pin, 0); // Command mode
    gpio_put(lcd->pins.cs_pin, 0); // Select chip
    pcd8544_spi_send_byte(lcd, command);
    gpio_put(lcd->pins.cs_pin, 1); // Deselect chip
}

// Internal function to send data
void pcd8544_data(pcd8544_t *lcd, uint8_t data) {
    gpio_put(lcd->pins.dc_pin, 1); // Data mode
    gpio_put(lcd->pins.cs_pin, 0); // Select chip
    pcd8544_spi_send_byte(lcd, data);
    gpio_put(lcd->pins.cs_pin, 1); // Deselect chip
}

// Initialize PCD8544 LCD
void pcd8544_init(pcd8544_t *lcd, pcd8544_pins_t pins) {
    lcd->pins = pins;

    // Initialize SPI
    spi_init(lcd->pins.spi_port, 4000 * 1000); // 4 MHz SPI clock
    gpio_set_function(lcd->pins.sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(lcd->pins.mosi_pin, GPIO_FUNC_SPI);

    // Initialize GPIOs
    gpio_init(lcd->pins.cs_pin);
    gpio_set_dir(lcd->pins.cs_pin, GPIO_OUT);
    gpio_put(lcd->pins.cs_pin, 1); // Deselect chip initially

    gpio_init(lcd->pins.dc_pin);
    gpio_set_dir(lcd->pins.dc_pin, GPIO_OUT);

    gpio_init(lcd->pins.rst_pin);
    gpio_set_dir(lcd->pins.rst_pin, GPIO_OUT);

    // Reset the LCD
    gpio_put(lcd->pins.rst_pin, 0);
    sleep_ms(100);
    gpio_put(lcd->pins.rst_pin, 1);
    sleep_ms(100);

    // Initialize LCD with commands (refer to datasheet section 8.1 and 13)
    pcd8544_command(lcd, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTR); // H = 1 (Extended instruction set)
    pcd8544_command(lcd, PCD8544_SETVOP | 0x3F); // Set Vop (contrast) to 0x3F (adjust as needed, 0x3F is a common value)
    pcd8544_command(lcd, PCD8544_SETBIAS | 0x03); // Set Bias system (BSx) to 1:48 (0x03 is common)
    pcd8544_command(lcd, PCD8544_FUNCTIONSET); // H = 0 (Basic instruction set)
    pcd8544_command(lcd, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL); // Display on, normal mode

    // Clear buffer and display
    pcd8544_clear(lcd);
    pcd8544_display_update(lcd);
}

// Update the entire display from the buffer
void pcd8544_display_update(pcd8544_t *lcd) {
    pcd8544_command(lcd, PCD8544_SETYADDR | 0); // Set Y address to 0
    pcd8544_command(lcd, PCD8544_SETXADDR | 0); // Set X address to 0

    gpio_put(lcd->pins.dc_pin, 1); // Data mode
    gpio_put(lcd->pins.cs_pin, 0); // Select chip

    for (int i = 0; i < (LCD_WIDTH * LCD_HEIGHT / 8); i++) {
        pcd8544_spi_send_byte(lcd, lcd->buffer[i]);
    }

    gpio_put(lcd->pins.cs_pin, 1); // Deselect chip
}

// Clear the display buffer
void pcd8544_clear(pcd8544_t *lcd) {
    for (int i = 0; i < (LCD_WIDTH * LCD_HEIGHT / 8); i++) {
        lcd->buffer[i] = 0x00;
    }
}

// Draw a single pixel
void pcd8544_draw_pixel(pcd8544_t *lcd, uint8_t x, uint8_t y, uint8_t color) {
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) {
        return; // Out of bounds
    }

    uint16_t index = x + (y / 8) * LCD_WIDTH;
    if (color) {
        lcd->buffer[index] |= (1 << (y % 8));
    } else {
        lcd->buffer[index] &= ~(1 << (y % 8));
    }
}

// Draw a line using Bresenham's algorithm
void pcd8544_draw_line(pcd8544_t *lcd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        pcd8544_draw_pixel(lcd, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// Draw a rectangle
void pcd8544_draw_rectangle(pcd8544_t *lcd, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color, uint8_t fill) {
    if (fill) {
        for (uint8_t i = x; i < x + w; i++) {
            for (uint8_t j = y; j < y + h; j++) {
                pcd8544_draw_pixel(lcd, i, j, color);
            }
        }
    } else {
        pcd8544_draw_line(lcd, x, y, x + w - 1, y, color);
        pcd8544_draw_line(lcd, x, y + h - 1, x + w - 1, y + h - 1, color);
        pcd8544_draw_line(lcd, x, y, x, y + h - 1, color);
        pcd8544_draw_line(lcd, x + w - 1, y, x + w - 1, y + h - 1, color);
    }
}

// Clear a rectangular area (draws a filled blank rectangle)
void pcd8544_clear_area(pcd8544_t *lcd, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    pcd8544_draw_rectangle(lcd, x, y, w, h, 0, 1); // Draw filled black rectangle
}

// Draw a character (6x8 pixels)
void pcd8544_draw_char(pcd8544_t *lcd, uint8_t x, uint8_t y, char c, uint8_t color) {
    if (x > LCD_WIDTH - 6 || y > LCD_HEIGHT - 8) return; // Out of bounds

    for (uint8_t i = 0; i < 5; i++) { // 5 columns for character data
        uint8_t line = font[(uint8_t)c - 0x20][i];
        for (uint8_t j = 0; j < 8; j++) { // 8 rows
            if ((line >> j) & 0x01) {
                pcd8544_draw_pixel(lcd, x + i, y + j, color);
            } else {
                pcd8544_draw_pixel(lcd, x + i, y + j, !color); // Clear pixel if color is 0
            }
        }
    }
    // Draw a blank column after each character for spacing
    for (uint8_t j = 0; j < 8; j++) {
        pcd8544_draw_pixel(lcd, x + 5, y + j, !color);
    }
}

// Draw a string
void pcd8544_draw_string(pcd8544_t *lcd, uint8_t x, uint8_t y, const char *str, uint8_t color) {
    uint8_t current_x = x;
    while (*str) {
        pcd8544_draw_char(lcd, current_x, y, *str, color);
        current_x += 6; // 6 pixels per character (5 data + 1 space)
        if (current_x + 6 > LCD_WIDTH) { // Wrap to next line if necessary
            current_x = x;
            y += 8; // Move down one character height
            if (y + 8 > LCD_HEIGHT) break; // Stop if out of display area
        }
        str++;
    }
}


