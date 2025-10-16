#pragma once

#include <allegro5/allegro.h>

#include <memory>

class Display {
    struct deleter {
        void operator()(ALLEGRO_DISPLAY* display) const;
    };

    std::unique_ptr<ALLEGRO_DISPLAY, deleter> m_display;
public:
    void create();
    void destroy();

    ALLEGRO_DISPLAY* get() const;

    void set_title(const char* title);
};