#include "table.h"

#include <random>
#include <cmath>

constexpr size_t _calculate_real_idx(const size_t x, const size_t y, const size_t orig_x) { return x + orig_x * y; }
constexpr float _calculate_real_coord(const size_t i, const float factor) { return (i + 0.5f) * factor - static_cast<float>(DEFAULT_SCREEN_SIDE_SIZE); }


Bubble* Table::get_at(const size_t x, const size_t y)
{
    return nullptr;
}

Table::Table(Resources& resources, const size_t x, const size_t y)
    : m_resources(resources),
      m_bubbles(std::shared_ptr<pBubble[]>(new pBubble[x*y])),
      m_common_bmp(resources.get_bitmap("assets/bubble.png")),
      m_orig_x(x), m_orig_y(y),
      m_x_factor(static_cast<float>(DEFAULT_SCREEN_SIDE_SIZE) * 2.0f / x),
      m_y_factor(static_cast<float>(DEFAULT_SCREEN_SIDE_SIZE) * 2.0f / y),
      m_scale_res(static_cast<float>(DEFAULT_SCREEN_SIDE_SIZE) * 1.8f / (
            std::max(x, y) * static_cast<float>(al_get_bitmap_width(m_common_bmp))
        ))
{
    random_fill(2);
}

void Table::random_fill(const size_t level)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    const size_t real_level = level > (static_cast<size_t>(BubbleColors::Color::_SIZE) - 1)
        ? static_cast<size_t>(BubbleColors::Color::_SIZE) : (level < 2 ? 2 : (level + 1));

    std::shared_ptr<pBubble[]> new_bubbles = std::shared_ptr<pBubble[]>(new pBubble[m_orig_x * m_orig_y]);

    for(size_t y = 0; y < m_orig_y; ++y) {
        for(size_t x = 0; x < m_orig_x; ++x) {
            const BubbleColors::Color random_color = 
                static_cast<BubbleColors::Color>(gen() % real_level);

            const auto p = _calculate_real_idx(x, y, m_orig_x);

            new_bubbles[p] = std::unique_ptr<Bubble>(
                new Bubble(
                    m_common_bmp,
                    _calculate_real_coord(x, m_x_factor),
                    _calculate_real_coord(y, m_y_factor),
                    m_scale_res,
                    random_color
                )
            );
        }
    }

    m_bubbles.exchange(std::move(new_bubbles), std::memory_order::release);
}

void Table::mouse_click_at(const float x, const float y)
{
    // Note: < 0 should not be used, this "floor" return 0 from (-1..1) -> skips one on -1
    const int real_px = static_cast<int>(floor((x + static_cast<float>(DEFAULT_SCREEN_SIDE_SIZE) - 1e-12f) / m_x_factor));
    const int real_py = static_cast<int>(floor((y + static_cast<float>(DEFAULT_SCREEN_SIDE_SIZE) - 1e-12f) / m_x_factor));

    if (real_px < 0 || static_cast<size_t>(real_px) >= m_orig_x ||
        real_py < 0 || static_cast<size_t>(real_py) >= m_orig_y)
        return; // cannot be valid

    const size_t real_pos = _calculate_real_idx(static_cast<size_t>(real_px), static_cast<size_t>(real_py), m_orig_x);
    std::unique_ptr<Bubble>& bubble = m_bubbles.load()[real_pos];

    if (!bubble) return;

    const BubbleColors::Color click_color = bubble->get_color();

    std::vector<size_t> all_pos;

    recursive_pop_at(real_px, real_py, click_color, all_pos);

    if (all_pos.size() >= 3) {
        for(const auto& i : all_pos) {
            //printf("D:%zu\n", i);
            auto& it = m_bubbles.load()[i];
            it.reset();
        }
        apply_gravity();
    }

    return;
}

void Table::draw_all(const Display& display)
{
    for(size_t p = 0; p < m_orig_x * m_orig_y; ++p) {
        if (auto& bubble = m_bubbles.load()[p])
            bubble->draw(display);
    }
}

void Table::recursive_pop_at(const int x, const int y, const BubbleColors::Color& c, std::vector<size_t>& v)
{
    //printf("VER %i %i\n", x, y);

    if (x < 0 || static_cast<size_t>(x) >= m_orig_x ||
        y < 0 || static_cast<size_t>(y) >= m_orig_y)
        return; // cannot be valid

    const size_t real_pos = _calculate_real_idx(static_cast<size_t>(x), static_cast<size_t>(y), m_orig_x);
    for(const auto& quick : v) {
        if (quick == real_pos) return;
    }

    std::unique_ptr<Bubble>& bubble = m_bubbles.load()[real_pos];

    if (!bubble || bubble->get_color() != c) return;
    
    v.push_back(real_pos);

    recursive_pop_at(x + 1, y, c, v);
    recursive_pop_at(x - 1, y, c, v);
    recursive_pop_at(x, y + 1, c, v);
    recursive_pop_at(x, y - 1, c, v);    
}

void Table::apply_gravity()
{
    for (size_t cases = 1; cases > 0;) {
        cases = 0;

        for (size_t x = 0; x < m_orig_x; ++x) {
            for (size_t y = 1; y < m_orig_y; ++y) {
                const size_t p = _calculate_real_idx(static_cast<size_t>(x), static_cast<size_t>(y), m_orig_x);
                const size_t p_up = _calculate_real_idx(static_cast<size_t>(x), static_cast<size_t>(y - 1), m_orig_x);
                
                auto& o = m_bubbles.load()[p];
                auto& o_up = m_bubbles.load()[p_up];

                if (o_up && !o) {
                    o = std::move(o_up);
                    o->move_to(
                        _calculate_real_coord(x, m_x_factor),
                        _calculate_real_coord(y, m_y_factor)
                    );
                    //printf("x %i %i -> %i %i\n", x, y - 1, x, y);
                    ++cases;
                }
            }
        }
    }

    for (size_t cases = 1; cases > 0;) {
        cases = 0;
        for (size_t x = 0; x < m_orig_x - 1; ++x) {
            size_t score = 0;
            for (size_t y = 0; y < m_orig_y; ++y) {
                const size_t p = _calculate_real_idx(static_cast<size_t>(x), static_cast<size_t>(y), m_orig_x);
                if (!m_bubbles.load()[p])
                    ++score;
            }
            if (score == m_orig_y) {
                for (size_t y = 0; y < m_orig_y; ++y) {
                    const size_t p = _calculate_real_idx(static_cast<size_t>(x), static_cast<size_t>(y), m_orig_x);
                    const size_t p_r = _calculate_real_idx(static_cast<size_t>(x + 1), static_cast<size_t>(y), m_orig_x);

                    auto& o = m_bubbles.load()[p];
                    auto& o_r = m_bubbles.load()[p_r];

                    if (o_r) {
                        o = std::move(o_r);
                        o->move_to(
                            _calculate_real_coord(x, m_x_factor),
                            _calculate_real_coord(y, m_y_factor)
                        );
                        //printf("| %i %i -> %i %i\n", x + 1, y, x, y);
                        ++cases;
                    }
                }
            }
        }
    }
}