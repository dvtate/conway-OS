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
    ConwayGame game{25, 80};

    // 4x4 block stable
    game.m_game.set(20, 20, true);
    game.m_game.set(20, 21, true);
    game.m_game.set(21, 20, true);
    game.m_game.set(21, 21, true);

    // Spinner
    game.m_game.set(15, 10, true);
    game.m_game.set(15, 11, true);
    game.m_game.set(15, 12, true);
    
    // Octagon 2
    game.m_game.set(5, 1, true);
    game.m_game.set(6, 1, true);
    game.m_game.set(4, 2, true);
    game.m_game.set(7, 2, true);
    game.m_game.set(3, 3, true);
    game.m_game.set(8, 3, true);
    game.m_game.set(2, 4, true);
    game.m_game.set(9, 4, true);
    game.m_game.set(2, 5, true);
    game.m_game.set(9, 5, true);
    game.m_game.set(3, 6, true);
    game.m_game.set(8, 6, true);
    game.m_game.set(4, 7, true);
    game.m_game.set(7, 7, true);
    game.m_game.set(5, 8, true);
    game.m_game.set(6, 8, true);

    for (;;)
        game.tick();
}