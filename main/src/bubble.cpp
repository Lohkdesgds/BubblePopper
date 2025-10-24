#include "bubble.h"

Bubble::Bubble(ALLEGRO_BITMAP* texture, const int pos_x, const int pos_y, const float scale, ALLEGRO_COLOR tint)
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

void Bubble::draw(const Display& display)
{
    const auto delta_secs = std::min(display.get_frame_time_ms() / 1000.0, 0.05) * 20.0;
    
    m_draw_x = static_cast<float>(m_draw_x * (1.0 - delta_secs) + m_grid_x * 1.0f * (delta_secs));
    m_draw_y = static_cast<float>(m_draw_y * (1.0 - delta_secs) + m_grid_y * 1.0f * (delta_secs));

    al_draw_tinted_scaled_rotated_bitmap(m_texture, m_tint,
        m_texture_size_x / 2, m_texture_size_y / 2,
        m_draw_x, m_draw_y, m_draw_scale, m_draw_scale, 0, 0
    );
}