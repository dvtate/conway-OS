
#include <cinttypes>

/**
 * Invoked by bootloader
 */
extern "C" void bootmain(void)
{
    // Print ok to frame buffer
    // char* fb = (char*) 0xb8000;
    // fb[0] = 'O';
    // fb[1] = 0x0f;
    // fb[2] = 'K';
    // fb[3] = 0x0f;
    uint16_t* vga = (uint16_t*) 0xb8000;
    for (int i = 0; i < 25*80; i++) {
        vga[i] = ' ' | (7 << 8);
    }

    // Halt loop
    while (1)
        asm("hlt");
}