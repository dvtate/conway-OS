
/**
 * This allocates space on the heap. Note that there is no corresponding free() implementation
 */
void* malloc(unsigned long nbytes) {
    // this could brick your computer!
    static char* ret = (char*) 0xFEB00000;
    return ret -= nbytes;
}
