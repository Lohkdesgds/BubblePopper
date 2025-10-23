#include "bubble.h"

Bubble::Bubble(ALLEGRO_BITMAP* texture, const int pos_x, const int pos_y)
    : m_texture(texture), grid_x(pos_x), grid_y(pos_y),
    m_texture_size_x(al_get_bitmap_width(texture)),
    m_texture_size_y(al_get_bitmap_height(texture)),
    draw_x(static_cast<int>(grid_x)), draw_y(static_cast<int>(grid_y))
{
}

void Bubble::move_to(const int new_x, const int new_y)
{
    grid_x = new_x;
    grid_y = new_y;
}

int Bubble::get_x() const
{
    return grid_x;
}

int Bubble::get_y() const
{
    return grid_y;
}

void Bubble::draw(const Display& display)
{
    const auto delta_secs = std::min(display.get_frame_time_ms() / 1000.0, 0.05) * 20.0;
    
    draw_x = static_cast<float>(draw_x * (1.0 - delta_secs) + grid_x * 1.0f * (delta_secs));
    draw_y = static_cast<float>(draw_y * (1.0 - delta_secs) + grid_y * 1.0f * (delta_secs));

    //printf("%.6lf | %03.3f %03.3f > %i %i \n", delta_secs, draw_x, draw_y, grid_x, grid_y);

    al_draw_scaled_rotated_bitmap(m_texture,
        m_texture_size_x / 2, m_texture_size_y / 2,
        draw_x, draw_y, 1.0f, 1.0f, 0, 0
    );
}