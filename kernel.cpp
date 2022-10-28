
#include <cinttypes>


#include "./include/crt.hpp"


/**
 * Application Model for conway's game of life
 */
struct ConwayGame {
    // Dimensions
    unsigned m_x, m_y;

    // Densly packed grids
    uint8_t* m_grid, * m_grid2;

    /**
     * Constructor
     */
    ConwayGame(unsigned x, unsigned y): m_x(x), m_y(y) {
        const unsigned long size = (unsigned long) x * (unsigned long) y >> 3lu;
        m_grid = (uint8_t*) malloc(size);
        m_grid2 = (uint8_t*) malloc(size);
    }

    inline unsigned long raw_ind(const unsigned x, const unsigned y) {
        return (unsigned long) x * (unsigned long) m_y + y;
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
    void set(const unsigned long long raw_ind, const bool v) {
        if (v) {
            m_grid[raw_ind >> 3] |= 0b10000000 >> (raw_ind % 8);
        } else {
            m_grid[raw_ind >> 3] &= ~(0b10000000 >> (raw_ind % 8));
        }
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
        if (y < m_y)
            ret += at(x, y + 1);
        if (x) {
            ret += at(x - 1, y);
            if (y)
                ret += at(x - 1, y - 1);
            if (y < m_y)
                ret += at(x - 1, y + 1);
        }
        if (x < m_x) {
            ret += at(x + 1, y);
            if (y)
                ret += at(x + 1, y - 1);
            if (y < m_y)
                ret += at(x + 1, y + 1);
        }
        return ret;
    }

    bool updated_cell(const unsigned x, const unsigned y) {
        const auto ns = neighbors(x, y);
        const bool cv = at(x, y);
        return (!cv && ns == 3) || (cv && (ns == 2 || ns == 3));
    }

    void update() {
        unsigned long i = 0;
        for (unsigned x = 0; x < m_x; x++)
            for (unsigned y = 0; y < m_y; y += 8) {
                uint8_t block = 0;
                if (updated_cell(x, y + 0))
                    block |= 0b10000000;
                if (updated_cell(x, y + 1))
                    block |= 0b1000000;
                if (updated_cell(x, y + 2))
                    block |= 0b100000;
                if (updated_cell(x, y + 3))
                    block |= 0b10000;
                if (updated_cell(x, y + 4))
                    block |= 0b1000;
                if (updated_cell(x, y + 5))
                    block |= 0b100;
                if (updated_cell(x, y + 6))
                    block |= 0b10;
                if (updated_cell(x, y + 7))
                    block |= 0b1;
                m_grid2[i++] = block;
            }

        swap_grids();
    }

    inline void swap_grids() {
        auto g = m_grid;
        m_grid = m_grid2;
        m_grid2 = g;
    }
};

struct GameView {
    // Game that we are viewing
    ConwayGame m_game;

    // Viewport dimensions
    unsigned m_wx, m_wy;

    // Viewport location
    unsigned m_x{0}, m_y{0};

    // Where is the user cursor
    // ?? Relative or absolute
    unsigned m_cursor_x, m_cursor_y;

    // Is the cursor illuminated this frame?
    bool m_cursor_lit{false};

    GameView(ConwayGame game, unsigned viewport_width, unsigned viewport_height):
        m_game(game), m_wx(viewport_width), m_wy(viewport_height)
    {
        m_cursor_x = m_x + m_wx / 2;
        m_cursor_y = m_y + m_wy / 2;
    }

    void move_cursor(int dx, const int dy) {
        if (dx < 0) {
            if (m_cursor_x > -dx) {
                m_cursor_x - (unsigned)(-dx);
            } else {
                dx += m_cursor_x;
                m_cursor_x = 0;
                if (m_x > 0 && dx != 0) {
                    if (dx > m_x)
                        m_x = 0;
                    else
                        m_x -= (unsigned)(-dx);
                }
            }
        } else if (dx > m_wx) {

        }
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
    "[WASD]         - move camera"
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