#pragma once

#include <allegro5/allegro.h>

#include <resources.h>
#include <display.h>

class Bubble {
public:
    Bubble(ALLEGRO_BITMAP*, const int, const int, const float = 1.0f, ALLEGRO_COLOR = al_map_rgb(255,255,255));

    void move_to(const int, const int);
    int get_x() const;
    int get_y() const;

    void draw(const Display&); // uses display.get_frame_time_ms() for draw_x, draw_y
private:
    ALLEGRO_BITMAP* m_texture;
    const ALLEGRO_COLOR m_tint;
    const int m_texture_size_x, m_texture_size_y;
    float m_draw_x, m_draw_y;
    const float m_draw_scale;
    int m_grid_x, m_grid_y;
};


namespace BubbleColors {
    constexpr ALLEGRO_COLOR _colors[] = {
        { 1.0f, 0.1f, 0.1f, 1.0f },
        { 0.1f, 1.0f, 0.1f, 1.0f },
        { 0.1f, 0.1f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 0.1f, 1.0f },
        { 1.0f, 0.1f, 1.0f, 1.0f },
        { 0.1f, 1.0f, 1.0f, 1.0f },
    };

    enum class Color {
        RED,
        GREEN,
        BLUE,
        YELLOW,
        PINK,
        AQUA,
        _SIZE
    };

    constexpr ALLEGRO_COLOR get(const Color& opt) noexcept { return _colors[static_cast<size_t>(opt) % static_cast<size_t>(Color::_SIZE)]; };
}