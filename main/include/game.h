#pragma once

#include <memory>

#include <resources.h>
#include <config.h>
#include <bubble.h>

#include <display.h>
#include <event_queue_wrapper.h>

class Game {
public:
    Game();
    ~Game();

    bool think();
    void draw();

    operator bool() const;
private:
    EventQueueWrapper m_event_queue_wrapper;
    Resources m_resources;
    Config m_config;
    Bubble bubble;
    std::unique_ptr<Display> m_display;
};
