#pragma once

#include <allegro5/allegro.h>

#include <resources.h>
#include <display.h>

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

    constexpr ALLEGRO_COLOR _get(const Color& opt) noexcept { return _colors[static_cast<size_t>(opt) % static_cast<size_t>(Color::_SIZE)]; };
}

class Bubble {
public:
    Bubble(ALLEGRO_BITMAP*, const int, const int, const float = 1.0f, BubbleColors::Color = BubbleColors::Color::RED);

    void move_to(const int, const int);
    int get_x() const;
    int get_y() const;
    BubbleColors::Color get_color() const;

    void draw(const Display&); // uses display.get_frame_time_ms() for draw_x, draw_y
private:
    ALLEGRO_BITMAP* m_texture;
    const float m_draw_scale;
    const int m_texture_size_x, m_texture_size_y;
    const BubbleColors::Color m_tint;
    float m_draw_x, m_draw_y;
    float m_vx = 0.0f, m_vy = 0.0f;
    int m_grid_x, m_grid_y;
};

