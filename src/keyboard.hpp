#include <cinttypes>

#include "crt.hpp"

template<class T, uint32_t N>
struct Queue {
    T m_buffer[N];
    uint32_t m_start{0};
    uint32_t m_end{0};


    uint32_t size() {
        if (m_start == m_end) {
            return 0;
        } else if (m_start < m_end) {
            return m_end - m_start;
        } else if (m_start > m_end) {
            return (N - m_start) + m_end;
        }
    }

    // Caller should check size() before calling these
    void push(T v) {
        if (m_start == m_end)
            return; // full, don't overwrite older values
        m_buffer[m_end] = v;
        if (++m_end > N)
            m_end = 0;
    }
    T& get() {
        return m_buffer[m_start];
    }
    T&& pop() {
        const auto s = m_start;
        if (++m_start > N)
            m_start = 0;
        return m_buffer[s];
    }
};

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
    Queue<Key, 512> buffer;

    void init() {
        // setup irq handler somehow
        // idt entry and more pain
    }

    void irq_01() {
        auto code = (Key) inb(0x60);
        buffer.push(code);
    }
};