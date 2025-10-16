#include "config.h"

#include <allegro5/allegro.h>

#include <stdexcept>
#include <filesystem>
#include <fstream>

#include <lunaris/console/console.h>

Config::Config() {
    ALLEGRO_PATH* user_config_path = al_get_standard_path(ALLEGRO_USER_SETTINGS_PATH);
    if (!user_config_path) {
        throw std::runtime_error("Failed to get user config path");
    }

    const std::filesystem::path config_file = std::filesystem::path(al_path_cstr(user_config_path, '/')) / "config.json";
    al_destroy_path(user_config_path);
    
    // if not exists and cannot create, throw
    if (!std::filesystem::exists(config_file.parent_path()) && !std::filesystem::create_directories(config_file.parent_path())) {
        Lunaris::cout << Lunaris::console::color::RED << "Failed to create config directory: " << config_file.parent_path().string();
        throw std::runtime_error("Failed to create config directory");
    }

    Lunaris::cout << "Using config file: " << config_file.string();

    if (std::filesystem::exists(config_file)) {
        // Load existing configuration
        std::ifstream file(config_file);
        if (!file.is_open()) {
            Lunaris::cout << Lunaris::console::color::RED << "Failed to open config file: " << config_file.string();
            throw std::runtime_error("Failed to open config file");
        }

        try {
            file >> m_persistent_config;
        } catch (const nlohmann::json::parse_error& e) {
            Lunaris::cout << Lunaris::console::color::RED << "Failed to parse config file: " << e.what();
            throw std::runtime_error(std::string("Failed to parse config file: ") + e.what());
        }
    }
    else {
        // Create default configuration
        m_persistent_config = {
            { "screen_width", 1920 },
            { "screen_height", 1080 },
            { "fullscreen", false },
            { "vsync", true },
            { "volume", 75 }
        };

        // Save default configuration
        std::ofstream file(config_file);
        if (!file.is_open()) {
            Lunaris::cout << Lunaris::console::color::RED << "Failed to create config file: " << config_file.string();
            throw std::runtime_error("Failed to create config file");
        }
        file << m_persistent_config.dump(4);
    }

    m_volatile_config = {
        { "debug_mode", false },
        { "config_file", config_file.string() }
    };

    Lunaris::cout << Lunaris::console::color::GREEN << "Configuration loaded successfully!";
}

Config::~Config() {
    std::ofstream file(m_volatile_config["config_file"].get<std::string>());
    if (file.is_open()) {
        file << m_persistent_config.dump(4);
        Lunaris::cout << Lunaris::console::color::GRAY << "Configuration saved and destroyed.";
    }
    else {
        Lunaris::cout << Lunaris::console::color::YELLOW << "Warning: Failed to save config file on destruction!";
    }
}

// === Sets === //

void Config::set_screen_width(int width) {
    m_persistent_config["screen_width"] = width;
}

void Config::set_screen_height(int height) {
    m_persistent_config["screen_height"] = height;
}

void Config::set_fullscreen(bool fullscreen) {
    m_persistent_config["fullscreen"] = fullscreen;
}

void Config::set_vsync(bool vsync) {
    m_persistent_config["vsync"] = vsync;
}

void Config::set_volume(int volume) {
    m_persistent_config["volume"] = volume;
}

void Config::set_debug_mode(bool debug_mode) {
    m_volatile_config["debug_mode"] = debug_mode;
}

// === Gets === //

int Config::get_screen_width() const {
    return m_persistent_config.value("screen_width", 0);
}

int Config::get_screen_height() const {
    return m_persistent_config.value("screen_height", 0);
}

bool Config::is_fullscreen() const {
    return m_persistent_config.value("fullscreen", true);
}

bool Config::is_vsync() const {
    return m_persistent_config.value("vsync", true);
}

int Config::get_volume() const {
    return m_persistent_config.value("volume", 75);
}

bool Config::is_debug_mode() const {
    return m_volatile_config.value("debug_mode", false);
}


Config::Config(const nlohmann::json& volatile_config, const nlohmann::json& persistent_config)
    : m_volatile_config(volatile_config), m_persistent_config(persistent_config)
{
    Lunaris::cout << Lunaris::console::color::YELLOW << "Warning: Config constructed with custom JSONs, this is not the default behavior!";
}