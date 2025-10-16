#include <event_queue_wrapper.h>

#include <stdexcept>

#include <lunaris/console/console.h>

EventQueueWrapper::EventQueueWrapper() {
    m_event_queue = al_create_event_queue();
    if (!m_event_queue) {
        Lunaris::cout << Lunaris::console::color::RED << "Failed to create event queue!";
        throw std::runtime_error("Failed to create event queue");
        return;
    }

    al_init_user_event_source(&m_custom_event_source);
    al_register_event_source(m_event_queue, &m_custom_event_source);

    Lunaris::cout << Lunaris::console::color::GREEN << "Event queue created successfully!";
}

EventQueueWrapper::~EventQueueWrapper() {
    if (m_event_queue) {
        al_destroy_event_queue(m_event_queue);
        m_event_queue = nullptr;
    }
    al_destroy_user_event_source(&m_custom_event_source);

    Lunaris::cout << Lunaris::console::color::GRAY << "Event queue destroyed.";
}

bool EventQueueWrapper::register_source(ALLEGRO_EVENT_SOURCE* source) {
    if (!m_event_queue || !source)
        return false;
    al_register_event_source(m_event_queue, source);
    return true;
}

bool EventQueueWrapper::unregister_source(ALLEGRO_EVENT_SOURCE* source) {
    if (!m_event_queue || !source)
        return false;
    al_unregister_event_source(m_event_queue, source);
    return true;
}

ALLEGRO_EVENT EventQueueWrapper::wait() {
    ALLEGRO_EVENT ev;
    if (!m_event_queue) {
        ev.type = 0;
        return ev;
    }
    al_wait_for_event(m_event_queue, &ev);
    return ev;
}

bool EventQueueWrapper::wait_timed(ALLEGRO_EVENT* ev, double seconds) {
    if (!m_event_queue || !ev)
        return false;
    return al_wait_for_event_timed(m_event_queue, ev, static_cast<float>(seconds));
}

void EventQueueWrapper::push_event(const ALLEGRO_EVENT& ev) {
    if (!m_event_queue)
        return;
    al_emit_user_event(&m_custom_event_source, const_cast<ALLEGRO_EVENT*>(&ev), nullptr);
}

EventQueueWrapper::operator bool() const {
    return m_event_queue != nullptr;
}
