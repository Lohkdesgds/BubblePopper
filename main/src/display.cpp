#include "display.h"

#include <stdexcept>

#include <lunaris/console/console.h>

Display::Display(Config& config)
    : m_config(config)
{
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_RESIZABLE | (m_config.is_fullscreen() ? ALLEGRO_FULLSCREEN_WINDOW : 0));
    al_set_new_display_option(ALLEGRO_VSYNC, m_config.is_vsync() ? 1 : 2, ALLEGRO_SUGGEST);

    m_display = al_create_display(m_config.get_screen_width(), m_config.get_screen_height());

    if (!m_display) {
        Lunaris::cout << Lunaris::console::color::RED << "Failed to create display! Config: "
            << m_config.get_screen_width() << "x" << m_config.get_screen_height()
            << (m_config.is_fullscreen() ? " fullscreen" : "") << (m_config.is_vsync() ? " vsync" : "");
        throw std::runtime_error("Failed to create display");
    }

    m_event_queue_wrapper.register_source(al_get_display_event_source(m_display));
    m_event_queue_wrapper.register_source(al_get_keyboard_event_source());
    
    m_event_thread = std::thread(&Display::handle_events, this);

    Lunaris::cout << Lunaris::console::color::GREEN << "Display created successfully! "
        << al_get_display_width(m_display) << "x" << al_get_display_height(m_display)
        << (is_fullscreen() ? " fullscreen" : "") << (m_config.is_vsync() ? " vsync" : "");
}

Display::~Display() {
    signal_stop();

    m_event_thread.join();

    if (m_display) {
        al_destroy_display(m_display);
        m_display = nullptr;
        m_closed = true;
    }

    Lunaris::cout << Lunaris::console::color::GRAY << "Display destroyed.";
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

Display::operator ALLEGRO_EVENT_SOURCE*() const {
    return m_display ? al_get_display_event_source(m_display) : nullptr;
}

void Display::flip() {
    if (m_display) {
        const auto now = std::chrono::high_resolution_clock::now();
        m_frame_time_ms = std::chrono::duration<double, std::milli>(now - m_last_call).count();
        m_last_call = now;
        ++m_frames_counter;
        if (std::chrono::duration<double, std::milli>(now - m_last_fps_calc).count() >= 1000.0) {
            m_fps = std::exchange(m_frames_counter, 0);
            m_last_fps_calc = now;
        }

        al_flip_display();
    }
}

double Display::get_frame_time_ms() const {
    return m_frame_time_ms;
}

unsigned Display::get_fps() const {
    return m_fps;
}

void Display::handle_events() {
    ALLEGRO_EVENT event;
    while (*this) {
        if (m_event_queue_wrapper.wait_timed(&event, 0.1)) {
            switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                Lunaris::cout << Lunaris::console::color::YELLOW << "Display close event received. Closing display event handler.";
                m_closed = true;
                return;
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                al_acknowledge_resize(m_display);
                m_config.set_screen_width(al_get_display_width(m_display));
                m_config.set_screen_height(al_get_display_height(m_display));
                break;
            default:
                break;
            }
        }
    }
}

void Display::signal_stop() {
    ALLEGRO_EVENT ev;
    ev.user.type = ALLEGRO_EVENT_DISPLAY_CLOSE;

    Lunaris::cout << Lunaris::console::color::GRAY << "Signaling display event thread to stop...";
    m_event_queue_wrapper.push_event(ev);
}
