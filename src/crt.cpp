#include "crt.hpp"

void* malloc(unsigned long nbytes) {
    // this could brick your computer!
    static char* ret = (char*) 0xFEB00000;
    return ret -= nbytes;
}
