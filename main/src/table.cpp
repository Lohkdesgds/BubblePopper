#include "table.h"

#include <random>
#include <cmath>

constexpr size_t _calculate_real_idx(const size_t x, const size_t y, const size_t orig_x) { return x + orig_x * y; }


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
    random_fill();
}

void Table::random_fill()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shared_ptr<pBubble[]> new_bubbles = std::shared_ptr<pBubble[]>(new pBubble[m_orig_x * m_orig_y]);

    for(size_t y = 0; y < m_orig_y; ++y) {
        for(size_t x = 0; x < m_orig_x; ++x) {
            const ALLEGRO_COLOR random_color = BubbleColors::get(
                static_cast<BubbleColors::Color>(gen() % static_cast<size_t>(BubbleColors::Color::_SIZE))
            );
            const auto p = _calculate_real_idx(x, y, m_orig_x);

            new_bubbles[p] = std::unique_ptr<Bubble>(
                new Bubble(
                    m_common_bmp,
                    (x + 0.5f) * m_x_factor - static_cast<float>(DEFAULT_SCREEN_SIDE_SIZE),
                    (y + 0.5f) * m_y_factor - static_cast<float>(DEFAULT_SCREEN_SIDE_SIZE),
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

    bubble.reset();

    return;
}

void Table::draw_all(const Display& display)
{
    for(size_t p = 0; p < m_orig_x * m_orig_y; ++p) {
        if (auto& bubble = m_bubbles.load()[p])
            bubble->draw(display);
    }
}