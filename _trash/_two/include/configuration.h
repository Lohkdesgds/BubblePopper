#pragma once

#include <nlohmann/json.hpp>

class Configuration {
    nlohmann::json m_saveable;
    nlohmann::json m_non_saveable;
public:
    Configuration();
    ~Configuration();

    int get_display_width() const;
    int get_display_height() const;
    void set_display_width(int width);
    void set_display_height(int height);

    bool get_fullscreen() const;
    void set_fullscreen(bool fullscreen);

    void set_running(bool running);
    bool get_running() const;
};