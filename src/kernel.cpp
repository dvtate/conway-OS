#include <cinttypes>

#include "crt.hpp"          // C runtime
#include "game.hpp"         // Game logic

// TODO display game
// TODO register and connect controls



// Controls help menu
const char* controls =
    "Controls\n"
    "[space]        - start/stop simulation\n"
    "WASD           - move cursor\n"
    "X              - kill cell at cursor\n"
    "C              - spawn cell at cursor"
    ;


// Initialize anything that needs initializing
void init() {
    Keyboard::init();
}


/**
 * Invoked by bootloader
 */
extern "C" void bootmain(void) {
    // Initialize kernel
    init();

    // TODO splash screen with controls instructions

    // Run game
    ConwayGame game{80, 25};
    game.m_game.set(10, 10, true);
    game.m_game.set(10, 11, true);
    game.m_game.set(11, 10, true);
    game.m_game.set(11, 11, true);

    // VGA::puts("asdmklfasklfmklasfmklfadmklasdfmklafmklaf");
    // VGA::clear();
    // VGA::puts("fuck me");
    
    for (;;)
        for (char i = 0; ; i++) {
            // if (i == 0)
            //     VGA::clear();
            game.tick();
        }
}