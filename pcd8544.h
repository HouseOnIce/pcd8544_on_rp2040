#ifndef PCD8544_H
#define PCD8544_H

#include <stdint.h>
#include "hardware/spi.h"
#include "hardware/gpio.h"

// LCD dimensions
#define LCD_WIDTH   84
#define LCD_HEIGHT  48

// PCD8544 commands
#define PCD8544_FUNCTIONSET     0x20
#define PCD8544_DISPLAYCONTROL  0x08
#define PCD8544_SETYADDR        0x40
#define PCD8544_SETXADDR        0x80
#define PCD8544_SETTEMP         0x04
#define PCD8544_SETBIAS         0x10
#define PCD8544_SETVOP          0x80

// FunctionSet options
#define PCD8544_POWERDOWN       0x04
#define PCD8544_ENTRYMODE       0x02 // V = 0 for horizontal addressing, V = 1 for vertical addressing
#define PCD8544_EXTENDEDINSTR   0x01 // H = 1 for extended instruction set

// DisplayControl options
#define PCD8544_DISPLAYBLANK    0x00
#define PCD8544_DISPLAYNORMAL   0x04
#define PCD8544_DISPLAYALLON    0x01
#define PCD8544_DISPLAYINVERSE  0x05

// Pin definitions (example, adjust as needed for your board)
typedef struct {
    spi_inst_t *spi_port;
    uint sck_pin;
    uint mosi_pin;
    uint cs_pin;
    uint dc_pin;
    uint rst_pin;
} pcd8544_pins_t;

// LCD context structure
typedef struct {
    pcd8544_pins_t pins;
    uint8_t buffer[LCD_WIDTH * LCD_HEIGHT / 8]; // 504 bytes (84 * 48 / 8)
} pcd8544_t;

// Function prototypes
void pcd8544_init(pcd8544_t *lcd, pcd8544_pins_t pins);
void pcd8544_command(pcd8544_t *lcd, uint8_t command);
void pcd8544_data(pcd8544_t *lcd, uint8_t data);
void pcd8544_display_update(pcd8544_t *lcd);
void pcd8544_clear(pcd8544_t *lcd);
void pcd8544_draw_pixel(pcd8544_t *lcd, uint8_t x, uint8_t y, uint8_t color);
void pcd8544_draw_line(pcd8544_t *lcd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void pcd8544_draw_rectangle(pcd8544_t *lcd, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color, uint8_t fill);
void pcd8544_clear_area(pcd8544_t *lcd, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void pcd8544_draw_char(pcd8544_t *lcd, uint8_t x, uint8_t y, char c, uint8_t color);
void pcd8544_draw_string(pcd8544_t *lcd, uint8_t x, uint8_t y, const char *str, uint8_t color);

#endif // PCD8544_H


