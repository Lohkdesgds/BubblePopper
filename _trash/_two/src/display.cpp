#include "display.h"

#include <stdexcept>
#include <functional>

#include "configuration.h"

template<DisplayConfiguration T>
void Display<T>::display_deleter::operator()(ALLEGRO_DISPLAY* display) const {
    if (display) { al_destroy_display(display); }
    display = nullptr;
}

template<DisplayConfiguration T>
void Display<T>::event_deleter::operator()(ALLEGRO_EVENT_QUEUE* queue) const {
    if (queue) { al_destroy_event_queue(queue); }
    queue = nullptr;
}

template<DisplayConfiguration T>
void Display<T>::handle_events() {
    ALLEGRO_EVENT event;
    while (m_config.get_running()) {
        al_wait_for_event(m_event_queue.get(), &event);
        switch(event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            m_config.set_running(false);
            break;
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            al_acknowledge_resize(m_display.get());
            m_config.set_display_width(event.display.width);
            m_config.set_display_height(event.display.height);
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            if (event.keyboard.keycode == ALLEGRO_KEY_F11) {
                m_config.set_fullscreen(!m_config.get_fullscreen());
                al_toggle_display_flag(m_display.get(), ALLEGRO_FULLSCREEN_WINDOW, m_config.get_fullscreen());
            }
            break;
        };
    }
}


template<DisplayConfiguration T>
void Display<T>::create() {
    int flags = 
        (m_config.get_fullscreen() ? ALLEGRO_FULLSCREEN_WINDOW : 0) |
        ALLEGRO_RESIZABLE | ALLEGRO_OPENGL;

    al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);
    al_set_new_display_flags(flags);

    m_display.reset(al_create_display(m_config.get_display_width(), m_config.get_display_height()));
    if (!m_display) {
        throw std::runtime_error("Failed to create display");
    }

    al_set_window_title(m_display.get(), DEFAULT_TITLE);

    m_event_queue.reset(al_create_event_queue());
    if (!m_event_queue) {
        destroy();
        throw std::runtime_error("Failed to create event queue");
    }

    al_register_event_source(m_event_queue.get(), al_get_display_event_source(m_display.get()));
    al_register_event_source(m_event_queue.get(), al_get_keyboard_event_source());
    al_register_event_source(m_event_queue.get(), al_get_mouse_event_source());

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    m_event_handler = std::jthread(&Display::handle_events, this);
}

template<DisplayConfiguration T>
void Display<T>::destroy() {
    m_display.reset();
    m_event_queue.reset();
}

template<DisplayConfiguration T>
Display<T>::Display(T& config)
    : m_config(config)
{
    create();
}

template<DisplayConfiguration T>
Display<T>::~Display() {
    m_config.set_running(false);
    if (m_event_handler.joinable()) {
        m_event_handler.join();
    }
    destroy();
}

template class Display<Configuration>;