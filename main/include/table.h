#pragma once

#include <memory>
#include <mutex>

#include <display.h>
#include <bubble.h>
#include <resources.h>


class Table {
    Bubble* get_at(const size_t, const size_t);
public:
    Table(Resources&, const size_t, const size_t);

    void random_fill();

    void mouse_click_at(const float, const float);

    void draw_all(const Display&);

    //bool game_ended() const;
    //size_t balls_left() const;
private:
    void recursive_pop_at(const int, const int, const BubbleColors::Color&, std::vector<size_t>&);
    void apply_gravity();

    using pBubble = std::unique_ptr<Bubble>;

    Resources& m_resources;
    std::atomic<std::shared_ptr<pBubble[]>> m_bubbles;
    ALLEGRO_BITMAP* m_common_bmp;
    const size_t m_orig_x, m_orig_y;
    const float m_x_factor, m_y_factor;
    const float m_scale_res;
};