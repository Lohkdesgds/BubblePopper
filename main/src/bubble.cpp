#include "bubble.h"

Bubble::Bubble(ALLEGRO_BITMAP* texture, const int pos_x, const int pos_y, const float scale, BubbleColors::Color tint)
    : m_texture(texture), m_grid_x(pos_x), m_grid_y(pos_y),
    m_texture_size_x(al_get_bitmap_width(texture)),
    m_texture_size_y(al_get_bitmap_height(texture)),
    m_draw_x(static_cast<float>(pos_x)), m_draw_y(static_cast<float>(pos_y)),
    m_draw_scale(scale),
    m_tint(tint)
{
}

void Bubble::move_to(const int new_x, const int new_y)
{
    m_grid_x = new_x;
    m_grid_y = new_y;
}

int Bubble::get_x() const
{
    return m_grid_x;
}

int Bubble::get_y() const
{
    return m_grid_y;
}

BubbleColors::Color Bubble::get_color() const
{
    return m_tint;
}

void Bubble::draw(const Display& display)
{
    const auto delta_secs = std::min(display.get_frame_time_ms() / 1000.0, 0.05) * 20.0;
    
    constexpr float stiffness = 2.0f;
    constexpr float damping = 0.8f;

    float dx = m_grid_x - m_draw_x;
    float dy = m_grid_y - m_draw_y;

    m_vx = m_vx * damping + dx * stiffness * delta_secs;
    m_vy = m_vy * damping + dy * stiffness * delta_secs;

    m_draw_x += m_vx * delta_secs;
    m_draw_y += m_vy * delta_secs;

    if (fabs(dx) < 1e-2f && fabs(m_vx) < 1e-5f) m_draw_x = m_grid_x;
    if (fabs(dy) < 1e-2f && fabs(m_vy) < 1e-5f) m_draw_y = m_grid_y;

    al_draw_tinted_scaled_rotated_bitmap(m_texture, BubbleColors::_get(m_tint),
        m_texture_size_x / 2, m_texture_size_y / 2,
        m_draw_x, m_draw_y, m_draw_scale, m_draw_scale, 0, 0
    );
}