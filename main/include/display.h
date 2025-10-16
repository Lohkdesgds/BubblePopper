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
    
    Display(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(const Display&) = delete;
    Display& operator=(Display&&) = delete;
private:
    Config& m_config;
    ALLEGRO_DISPLAY* m_display = nullptr;
    EventQueueWrapper m_event_queue_wrapper;
    std::thread m_event_thread;
    bool m_closed = false;

    void handle_events();
    void signal_stop();
};

