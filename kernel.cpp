#include <cinttypes>


#include "./include/crt.hpp"


/**
 * Application Model for conway's game of life
 */
struct ConwayGame {
    // Dimensions
    const unsigned m_height, m_width;

    // Densly packed grids
    uint8_t* m_grid, * m_grid2;

    /**
     * Constructor
     */
    ConwayGame(const unsigned height, const unsigned width):
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

enum class Direction {
    NORTH, EAST, SOUTH, WEST,
};

struct GameView {
    // Game that we are viewing
    ConwayGame m_game;

    // Viewport dimensions
    unsigned m_width, m_height;

    // Viewport location
    unsigned m_x{0}, m_y{0};

    // Where is the user cursor
    // ?? Relative or absolute
    unsigned m_cursor_x, m_cursor_y;

    // Is the cursor illuminated this frame?
    bool m_cursor_lit{false};

    GameView(ConwayGame game, const unsigned viewport_height, const unsigned viewport_width):
        m_game(game), m_width(viewport_width), m_height(viewport_height)
    {
        m_cursor_x = m_width / 2;
        m_cursor_y = m_height / 2;
    }

    void show() {
        auto* p = m_game.m_grid;
        const auto* p_max = p + (m_game.m_height * m_game.m_width >> 3);
        unsigned x = 0;
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

    void frame() {
        show();
        m_game.update();
    }

    void move_cursor(Direction d) {
        switch (d) {
        case Direction::NORTH:
            if (m_cursor_y)
                m_cursor_y--;
            else if (m_y)
                m_y--;
            break;
        case Direction::EAST:
            if (m_cursor_x < m_width)
                m_cursor_x++;
            else if (m_x < m_game.m_width)
                m_x++;
            break;
        case Direction::SOUTH:
            if (m_cursor_y < m_height)
                m_cursor_y++;
            else if (m_y < m_game.m_height)
                m_y++;
            break;
        case Direction::WEST:
            if (m_cursor_x)
                m_cursor_x--;
            else if (m_x)
                m_x--;
            break;
        }
    }
    inline void spawn() {
        m_game.set(m_x + m_cursor_x, m_y + m_cursor_y, true);
    }
    inline void kill() {
        m_game.set(m_x + m_cursor_x, m_y + m_cursor_y, false);
    }
};

// TODO display game
// TODO register and connect controls

// Controls help menu
const char* controls =
    "Controls"
    "[space]        - start/stop simulation"
    "[arrows]       - move cursor"
    "X              - kill cell at cursor"
    "C              - spawn cell at cursor"
    ;

void init() {
    // ??
}


/**
 * Invoked by bootloader
 */
extern "C" void bootmain(void)
{
    init();
    ConwayGame game{80, 25};
    GameView view(game, 80, 25);

}