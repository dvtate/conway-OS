#pragma once

#define fence() __asm__ volatile ("":::"memory")

/**
 * This allocates space on the heap. Note that there is no corresponding free() implementation
 */
// TODO probably have to use multiboot bios to get the actual memory layout
void* malloc(unsigned long nbytes) {
    // This could brick your computer!
    static char* ret = (char*) 0xFEB00000;
    return ret -= nbytes;
}

/**
 * Write to port
 */
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

/**
 * Read from port
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

/**
 * Wait a short amount of time (1 to 4 microseconds, generally)
 */
static inline void io_wait(void) {
    outb(0x80, 0);
}