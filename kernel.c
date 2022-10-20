
/**
* Invoked by bootloader
*/
void bootmain(void)
{
    // Print ok to frame buffer
    char* fb = (char*) 0xb8000;
    fb[0] = 'O';
    fb[1] = 0x0f;
    fb[2] = 'K';
    fb[3] = 0x0f;

    // Halt loop
    while (1)
        asm("hlt");
}