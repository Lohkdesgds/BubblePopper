#pragma once

#include <allegro5/allegro.h>

#include <thread>

#include <config.h>
#include <event_queue_wrapper.h>

class Display {
public:
    Display(Config&);
    ~Display();

    void resize(int width, int height);
    bool toggle_fullscreen();

    int get_width() const;
    int get_height() const;
    bool is_fullscreen() const;

    operator bool() const;
    operator ALLEGRO_EVENT_SOURCE*() const;

    void flip();

    double get_frame_time_ms() const;
    unsigned get_fps() const;

    float get_draw_width() const;
    float get_draw_height() const;
    float get_draw_scale() const;
    
    Display(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(const Display&) = delete;
    Display& operator=(Display&&) = delete;
private:
    const int limits_min_side_calc = 1000; // min 1000 on small side.

    Config& m_config;
    ALLEGRO_DISPLAY* m_display = nullptr;
    ALLEGRO_TIMER* m_timer_fps = nullptr;
    ALLEGRO_TRANSFORM m_transform;
    EventQueueWrapper m_event_queue_wrapper;
    std::thread m_event_thread;
    std::chrono::high_resolution_clock::time_point m_last_call;
    double m_frame_time_ms = 0.0;
    unsigned m_frames_counter = 0;
    unsigned m_fps = 0;
    float m_draw_screen_x = 1000.0f, m_draw_screen_y = 1000.0f, m_draw_screen_scale = 1.0f;
    bool m_closed = false;
    bool m_update_transform = false;

    void handle_events();
    void signal_stop();
    void update_matrix();
};

