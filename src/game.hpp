#pragma once
#include <cinttypes>

#include "crt.hpp"
#include "keyboard.hpp"
#include "display.hpp"

/**
 * Application Model for conway's game of life
 */
struct ConwayGameModel {
    // Dimensions
    const unsigned m_height, m_width;

    // Densly packed grids
    uint8_t* m_grid, * m_grid2;

    /**
     * Constructor
     */
    ConwayGameModel(const unsigned height, const unsigned width):
        m_height(height), m_width(width)
    {
        const unsigned long size = (unsigned long) height * (unsigned long) width;
        m_grid = (uint8_t*) malloc(size);
        m_grid2 = (uint8_t*) malloc(size);
    }

    inline unsigned long raw_ind(const unsigned x, const unsigned y) {
        // if (y >= m_width)
        //     return (unsigned long) (x + 1) * (unsigned long) m_width + (unsigned long) (y % m_width);
        // else
            return (unsigned long) y * (unsigned long) m_width + x;
    }

    /**
     * Get value at given cell index
     */
    bool at(const unsigned long raw_ind) {
        return m_grid[raw_ind >> 3] & (0b10000000 >> (raw_ind % 8));
    }
    inline bool at(const unsigned x, const unsigned y) {
        return at(raw_ind(x, y));
    }

    /**
     * Set the value for cell at given index
     */
    void set(const unsigned long raw_ind, const bool v) {
        if (v)
            m_grid[raw_ind >> 3] |= 0b10000000 >> (raw_ind % 8);
        else
            m_grid[raw_ind >> 3] &= ~(0b10000000 >> (raw_ind % 8));
    }
    inline void set(const unsigned x, const unsigned y, const bool v) {
        return set(raw_ind(x, y), v);
    }

    /**
     * How many neighbors does the cell at x, y have?
     */
    uint8_t neighbors(unsigned x, unsigned y) {
        uint8_t ret = 0;
        if (y)
            ret += at(x, y - 1);
        if (y < m_height)
            ret += at(x, y + 1);
        if (x) {
            ret += at(x - 1, y);
            if (y)
                ret += at(x - 1, y - 1);
            if (y < m_height)
                ret += at(x - 1, y + 1);
        }
        if (x < m_width) {
            ret += at(x + 1, y);
            if (y)
                ret += at(x + 1, y - 1);
            if (y < m_height)
                ret += at(x + 1, y + 1);
        }
        return ret;
    }

    bool updated_cell(const unsigned x, const unsigned y) {
        const auto ns = neighbors(x, y);
        const bool cv = at(x, y);
        return (cv && ns == 2) || ns == 3;
    }

    bool updated_cell(const unsigned long raw_ind) {
        const bool cv = at(raw_ind);
        const auto x = raw_ind % m_width;
        const auto y = raw_ind / m_width;
        const auto ns = neighbors(x, y);
        return (cv && ns == 2) || ns == 3;
    }

    void update() {
        const unsigned long size = (unsigned long) m_width * (unsigned long) m_height;
        for (unsigned long i = 0; i < size;) {
            uint8_t block = 0;
            const auto idx = i >> 3;
            if (updated_cell(i++))
                block |= 0b10000000;
            if (updated_cell(i++))
                block |= 0b1000000;
            if (updated_cell(i++))
                block |= 0b100000;
            if (updated_cell(i++))
                block |= 0b10000;
            if (updated_cell(i++))
                block |= 0b1000;
            if (updated_cell(i++))
                block |= 0b100;
            if (updated_cell(i++))
                block |= 0b10;
            if (updated_cell(i++))
                block |= 0b1;
            m_grid2[idx] = block;
        }
        swap_grids();
    }

    inline void swap_grids() {
        auto g = m_grid;
        m_grid = m_grid2;
        m_grid2 = g;
    }
};

/**
* Application view
*/
struct ConwayGame {
    // Game that we are viewing
    ConwayGameModel m_game;

    // Viewport dimensions
    unsigned m_width, m_height;

    // Viewport location
    unsigned m_x{0}, m_y{0};

    // Where is the user cursor
    // ?? Relative or absolute
    unsigned m_cursor_x, m_cursor_y;

    // Is the cursor illuminated this frame?
    bool m_cursor_lit: 1 {false};

    // wasd states
    bool m_key_up: 1 {false};
    bool m_key_down: 1 {false};
    bool m_key_left: 1 {false};
    bool m_key_right: 1 {false};

    // Is the simulation currently paused
    bool m_paused: 1 {true};

    ConwayGame(const unsigned viewport_height, const unsigned viewport_width):
        m_game(viewport_height, viewport_width), m_width(viewport_width), m_height(viewport_height)
    {
        m_cursor_x = 0;// m_width / 2;
        m_cursor_y = 0;// m_height / 2;
    }

    void show() {
        unsigned i = 0;
        const auto i_max = m_game.m_height * m_game.m_width;

        unsigned x = 0;
        unsigned y = 0;

        // VGA::clear();
        do {
            x = i % m_width;
            y = i / m_height;
            bool live = m_game.at(i);
            if (x == m_cursor_x && y == m_cursor_y)
                VGA::putc(live ? 'X' : 'C');
            else
                VGA::putc(live ? '#' : '_');
        } while (++i < i_max);

        // TODO use this optimized algorthim instead
        // auto* p = m_game.m_grid;
        // const auto* p_max = p + (m_game.m_height * m_game.m_width >> 3);
        // do {
        //     for (uint8_t i = 0; i < 8; i++) {
        //         if (x == m_game.m_width) {
        //             x = 0;
        //             std::cout <<"\n";
        //         }
        //         std::cout <<( (*p & (0b1000'0000 >> i) ) ? '#' : '.');
        //         x++;
        //     }
        // } while (++p != p_max);
        // std::cout <<std::flush;
    }

    void cursor_up() {
        m_cursor_x--;
    }
    void cursor_down() {
        m_cursor_x++;
    }
    void cursor_right() {
        m_cursor_y++;
    }
    void cursor_left() {
        m_cursor_y--;
    }
    void put_cursor(const unsigned x, const unsigned y) {
        m_cursor_x = x;
        m_cursor_y = y;
    }

    void move_cursor(const int dx, const int dy) {
        if (dx < 0)
            if (m_cursor_x < dx)
                m_cursor_x = 0;
            else
                m_cursor_x -= dx;
        else
            if (m_cursor_x + dx > m_width)
                m_cursor_x = m_width - 1;
            else
                m_cursor_x += dx;

        if (dy < 0)
            if (m_cursor_y < dy)
                m_cursor_y = 0;
            else
                m_cursor_y -= dy;
        else
            if (m_cursor_y + dy > m_width)
                m_cursor_y = m_height - 1;
            else
                m_cursor_y += dy;
    }

    inline void spawn() {
        m_game.set(m_x + m_cursor_x, m_y + m_cursor_y, true);
    }
    inline void kill() {
        m_game.set(m_x + m_cursor_x, m_y + m_cursor_y, false);
    }

    void tick() {
        // Get keyboard input
        switch (Keyboard::get_key())
        {
            // Toggle pause
            case Keyboard::Key::SPACE_DOWN:
                m_paused = !m_paused;
                // VGA::putc('_');
                break;

            // Kill
            case Keyboard::Key::X_DOWN:
                kill();
                // VGA::putc('x');
                break;

            // Spawn
            case Keyboard::Key::C_DOWN:
                spawn();
                // VGA::putc('c');
                break;

            // WASD
            case Keyboard::Key::W_DOWN:
                m_key_up = true;
                // VGA::putc('w');
                break;
            case Keyboard::Key::W_UP:
                m_key_up = false;
                // VGA::putc('W');
                break;
            case Keyboard::Key::A_DOWN:
                m_key_left = true;
                // VGA::putc('a');
                break;
            case Keyboard::Key::A_UP:
                m_key_left = false;
                // VGA::putc('A');
                break;
            case Keyboard::Key::S_DOWN:
                m_key_down = true;
                // VGA::putc('s');
                break;
            case Keyboard::Key::S_UP:
                m_key_down = false;
                // VGA::putc('S');
                break;
            case Keyboard::Key::D_DOWN:
                m_key_right = true;
                // VGA::putc('d');
                break;
            case Keyboard::Key::D_UP:
                m_key_right = false;
                // VGA::putc('D');
                break;                
        }

        // Move cursor
        if (m_key_up && !m_key_down)
            cursor_up();
        if (m_key_down && !m_key_up)
            cursor_down();
        if (m_key_left && !m_key_right)
            cursor_left();
        if (m_key_right && !m_key_left)
            cursor_right();
        
        // Update game state
        if (!m_paused)
            m_game.update();

        // Display the game
        show();

        // Sleep?
        io_wait();
    }
};