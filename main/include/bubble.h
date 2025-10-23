#pragma once

#include <allegro5/allegro.h>

#include <resources.h>
#include <display.h>

class Bubble {
public:
    Bubble(ALLEGRO_BITMAP*, const int, const int);

    void move_to(const int, const int);
    int get_x() const;
    int get_y() const;

    void draw(const Display&); // uses display.get_frame_time_ms() for draw_x, draw_y
private:
    ALLEGRO_BITMAP* m_texture;
    const int m_texture_size_x, m_texture_size_y;
    int grid_x, grid_y;
    float draw_x, draw_y;
};