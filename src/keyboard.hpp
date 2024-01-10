#pragma once
#include <cinttypes>

#include "crt.hpp"
#include "display.hpp"

// template<class T, uint32_t N>
// struct Queue {
//     T m_buffer[N];
//     uint32_t m_start{0};
//     uint32_t m_end{0};


//     uint32_t size() {
//         if (m_start == m_end) {
//             return 0;
//         } else if (m_start < m_end) {
//             return m_end - m_start;
//         } else if (m_start > m_end) {
//             return (N - m_start) + m_end;
//         }
//     }

//     // Caller should check size() before calling these
//     void push(T v) {
//         if (m_start == m_end)
//             return; // full, don't overwrite older values
//         m_buffer[m_end] = v;
//         if (++m_end > N)
//             m_end = 0;
//     }
//     T& get() {
//         return m_buffer[m_start];
//     }
//     T&& pop() {
//         const auto s = m_start;
//         if (++m_start > N)
//             m_start = 0;
//         return m_buffer[s];
//     }
// };

/**
 * Singleton keybaord class
 */
namespace Keyboard {


    enum class Key : unsigned char {
        W_DOWN = 0x11,
        A_DOWN = 0x1E,
        S_DOWN = 0x1F,
        D_DOWN = 0x20,
        C_DOWN = 0x2E,
        X_DOWN = 0x2D,
        SPACE_DOWN = 0x39,

        W_UP = 0x11 + 128,
        A_UP = 0x1E + 128,
        S_UP = 0x1F + 128,
        D_UP = 0x20 + 128,
        C_UP = 0x2E + 128,
        X_UP = 0x2D + 128,
        SPACE_UP = 0x39 + 128,
    };

    // Keypress buffer
    // Queue<Key, 512> buffer;

    void init() {
        // TODO setup irq handler somehow
        // idt entry and more pain
    }

    Key get_key() {
        return (Key) inb(0x60);
    }

    void restart() {
        int data = inb(0x61);
        outb(0x61, data | 0x80); // Disable KB
        outb(0x61, data & 0x7F); // Enable KB
    }

    // void set_leds(bool numled, bool capsled, bool scrollled) 
    // {     
    //     constexpr char SCROLL_LED = 1; 
    //     constexpr char NUM_LED    = 2; 
    //     constexpr char CAPS_LED   = 4;
    //     unsigned char temp = 0;     
    //     temp = (scrollled) ? (temp | SCROLL_LED ) : (temp & SCROLL_LED);    
    //     temp = (numled) ?  (num | NUM_LED) : (num & NUM_LED);   
    //     temp = (capsled) ? (num | CAPS_LED) : (num & CAPS_LED); 
    //     while((inportb(0x64)&2)!=0){} //Wait for the keyboard controller to receive command 
    //     outportb(0x60,0xED);
    //     while((inportb(0x64)&2)!=0){} //Wait for the keyboard controller to receive command  
    //     outportb(0x60,temp); 
    // }
};