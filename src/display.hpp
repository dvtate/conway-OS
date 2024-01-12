#pragma once
#include "crt.hpp"
#include <cinttypes>

namespace VGA {
    // Frame buffer
    uint16_t* fb = (uint16_t*) 0xB8000;
    unsigned int vga_index = 0;

    // Terminal colors
    enum class Color : uint16_t {
	    BLACK = 0,
	    BLUE = 1,
	    GREEN = 2,
	    CYAN = 3,
	    RED = 4,
	    MAGENTA = 5,
	    BROWN = 6,
	    LIGHT_GREY = 7,
	    DARK_GREY = 8,
	    LIGHT_BLUE = 9,
	    LIGHT_GREEN = 10,
	    LIGHT_CYAN = 11,
	    LIGHT_RED = 12,
	    LIGHT_MAGENTA = 13,
	    LIGHT_BROWN = 14,
	    WHITE = 15,
    };

    static inline uint16_t vga_entry_color(const Color fg, const Color bg) {
        return (uint16_t) fg | (uint16_t)bg << 4;
    }
    
    // Clear the screen
    void clear(void) {
        for (int i = 0; i < 80 * 25; i++)
            fb[i] = (uint16_t)' ';
        vga_index = 0;
    }

    void overwrite() {
        vga_index = 0;
    }

    // Print a string
    void puts(const char* str, const Color color = Color::GREEN) {
        int index = 0;
        while (str[index])
            fb[vga_index++] = (uint16_t)str[index++] | (uint16_t)color << 8;
    }

    // Print a character
    void putc(char c, const Color color = Color::GREEN) {
        fb[vga_index++] = (uint16_t)c | (uint16_t)color << 8;    
    }

    // Print an int
    void putd(int n, const Color color = Color::GREEN) {
        static char rret[512];
        int digit = 0;
        if (n == 0)
            rret[0] = '0';
        for (; digit < 512 && n != 0; digit++) {
            rret[digit] = '0' + (n % 10);
            n /= 10;
        }
        for (; digit >= 0; digit--)
            putc(rret[digit], color);
    }

    // Put a char at a specific location
    void putc_at(char c, int x, int y, const Color color = Color::GREEN) {
        const int i = y * 80 + x;
        fb[i] = (uint16_t)c | (uint16_t)color << 8;
    }
} // namespace VGA