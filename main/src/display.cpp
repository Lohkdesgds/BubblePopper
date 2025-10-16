#include "display.h"

#include <stdexcept>

#include <lunaris/console/console.h>

Display::Display(int width, int height, bool fullscreen, bool vsync) {
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_RESIZABLE | (fullscreen ? ALLEGRO_FULLSCREEN_WINDOW : 0));
    al_set_new_display_option(ALLEGRO_VSYNC, vsync ? 1 : 2, ALLEGRO_SUGGEST);

    m_display = al_create_display(width, height);

    if (!m_display) {
        Lunaris::cout << Lunaris::console::color::RED << "Failed to create display! Config: " << width << "x" << height << ", fullscreen: " << (fullscreen ? "yes" : "no") << ", vsync: " << (vsync ? "yes" : "no");
        throw std::runtime_error("Failed to create display");
    }

    m_event_queue = al_create_event_queue();
    if (!m_event_queue) {
        al_destroy_display(m_display);
        m_display = nullptr;
        Lunaris::cout << Lunaris::console::color::RED << "Failed to create event queue!";
        throw std::runtime_error("Failed to create event queue");
    }

    al_register_event_source(m_event_queue, al_get_display_event_source(m_display));
    al_register_event_source(m_event_queue, al_get_keyboard_event_source());

    al_init_user_event_source(&m_custom_stop_event);
    al_register_event_source(m_event_queue, &m_custom_stop_event);
    
    m_event_thread = std::thread(&Display::handle_events, this);

}

Display::~Display() {
    signal_stop();

    m_event_thread.join();

    if (m_event_queue) {
        al_destroy_event_queue(m_event_queue);
        m_event_queue = nullptr;
    }

    al_destroy_user_event_source(&m_custom_stop_event);

    if (m_display) {
        al_destroy_display(m_display);
        m_display = nullptr;
    }
}


void Display::resize(int width, int height)
{
    if (m_display)
        al_resize_display(m_display, width, height);
}

bool Display::toggle_fullscreen()
{
    if (m_display) {
        const bool was_fullscreen = is_fullscreen();
        al_set_display_flag(m_display, ALLEGRO_FULLSCREEN_WINDOW, !was_fullscreen);
        return !was_fullscreen;
    }
    return false;
}

int  Display::get_width() const
{
    return m_display ? al_get_display_width(m_display) : 0;
}

int  Display::get_height() const
{
    return m_display ? al_get_display_height(m_display) : 0;
}

bool Display::is_fullscreen() const
{
    return m_display ? (al_get_display_flags(m_display) & ALLEGRO_FULLSCREEN_WINDOW) > 0 : false;
}

Display::operator bool() const {
    return !m_closed;
}

void Display::handle_events() {
    ALLEGRO_EVENT event;
    while (*this) {
        if (al_wait_for_event_timed(m_event_queue, &event, 0.1)) {
            switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                m_closed = true;
                return;
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                al_acknowledge_resize(m_display);
                break;
            default:
                break;
            }
        }
    }
}

void Display::signal_stop() {
    if (!m_event_queue) return;

    ALLEGRO_EVENT ev;
    ev.user.type = ALLEGRO_EVENT_DISPLAY_CLOSE;
    al_emit_user_event(&m_custom_stop_event, &ev, nullptr);
}
