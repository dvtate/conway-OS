#pragma once
#include <cinttypes>

namespace VGA {
    // Frame buffer
    uint16_t* fb = (uint16_t*) 0xB8000;
    unsigned int vga_index = 0;

    // Terminal colors
    enum class Color : uint16_t {
        BLACK = 0,
        GREEN = 2,
        RED = 4,
        YELLOW = 14,
        WHITE = 15,
    };

    // Clear the screen
    void clear(void) {
        for (int i = 0; i < 80 * 25; i++)
            fb[i] = (uint16_t)' ';
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

} // namespace VGA