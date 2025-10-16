#pragma once

#include <memory>

#include <resources.h>
#include <config.h>
#include <display.h>


class Game {
public:
    Game();
    ~Game();

    void run();
private:
    Resources m_resources;
    Config m_config;
    std::unique_ptr<Display> m_display;
};
