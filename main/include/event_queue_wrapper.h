#pragma once

#include <allegro5/allegro.h>

#include <thread>

class EventQueueWrapper {
public:
    EventQueueWrapper();
    ~EventQueueWrapper();

    EventQueueWrapper(const EventQueueWrapper&) = delete;
    EventQueueWrapper(EventQueueWrapper&&) = delete;
    EventQueueWrapper& operator=(const EventQueueWrapper&) = delete;
    EventQueueWrapper& operator=(EventQueueWrapper&&) = delete;

    bool register_source(ALLEGRO_EVENT_SOURCE*);
    bool unregister_source(ALLEGRO_EVENT_SOURCE*);

    ALLEGRO_EVENT wait();
    bool wait_timed(ALLEGRO_EVENT*, double seconds);
    bool get_event(ALLEGRO_EVENT*);

    void push_event(const ALLEGRO_EVENT&);

    operator bool() const;
private:
    ALLEGRO_EVENT_QUEUE* m_event_queue = nullptr;
    ALLEGRO_EVENT_SOURCE m_custom_event_source;
};