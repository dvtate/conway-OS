#pragma once



/**
 * This allocates space on the heap. Note that there is no corresponding free() implementation
 */
// TODO probably have to use multiboot bios to get the actual memory layout
void* malloc(unsigned long nbytes);