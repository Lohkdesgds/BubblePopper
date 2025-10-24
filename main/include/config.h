#pragma once

#include <nlohmann/json.hpp>

constexpr int DEFAULT_SCREEN_SIDE_SIZE = 1000;

class Config {
public:
    Config();
    ~Config();

    Config(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(const Config&) = delete;
    Config& operator=(Config&&) = delete;

    // === Sets === //
    void set_screen_width(int);
    void set_screen_height(int);
    void set_fullscreen(bool);
    void set_vsync(bool);
    void set_volume(int);

    void set_debug_mode(bool);


    // === Gets === //
    int get_screen_width() const;
    int get_screen_height() const;
    bool is_fullscreen() const;
    bool is_vsync() const;
    int get_volume() const;

    bool is_debug_mode() const;

private:
    nlohmann::json m_volatile_config;
    nlohmann::json m_persistent_config;
protected:
    Config(const nlohmann::json&, const nlohmann::json&);
};