#pragma once

#include <allegro5/allegro.h>

#include <thread>

class Display {
public:
    Display(int width, int height, bool fullscreen, bool vsync);
    ~Display();

    void resize(int width, int height);
    bool toggle_fullscreen();

    int get_width() const;
    int get_height() const;
    bool is_fullscreen() const;

    operator bool() const;
    
    Display(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(const Display&) = delete;
    Display& operator=(Display&&) = delete;
private:
    ALLEGRO_DISPLAY* m_display = nullptr;
    ALLEGRO_EVENT_QUEUE* m_event_queue = nullptr;
    ALLEGRO_EVENT_SOURCE m_custom_stop_event;
    std::jthread m_event_thread;

    void handle_events();
    void signal_stop();
    void destroy();
};

