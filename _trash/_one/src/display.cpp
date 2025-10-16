#include "display.h"

#include <stdexcept>

void Display::deleter::operator()(ALLEGRO_DISPLAY* display) const {
    if (display) {
        al_destroy_display(display);
    }
}

void Display::create() {
    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(0, &info);
    
    const int screen_width = info.x2 - info.x1;
    const int screen_height = info.y2 - info.y1;

    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_OPENGL);
    al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);

    m_display = std::unique_ptr<ALLEGRO_DISPLAY, deleter>(
        al_create_display(screen_width, screen_height)
    );

    if (!m_display) {
        throw std::runtime_error("Failed to create Allegro display");
    }

    al_set_window_title(m_display.get(), "Bubble Popper - Loading...");
}

void Display::destroy() {
    m_display.reset();
}

ALLEGRO_DISPLAY* Display::get() const {
    return m_display.get();
}

void Display::set_title(const char* title) {
    if (m_display) {
        al_set_window_title(m_display.get(), title);
    }
}