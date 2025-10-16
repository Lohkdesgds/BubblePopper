#pragma once

#include <concepts>
#include <memory>
#include <thread>

#include <allegro5/allegro.h>

template <typename T>
concept DisplayConfiguration = requires(T a, const T& b) {
    { b.get_display_width() } -> std::same_as<int>;
    { b.get_display_height() } -> std::same_as<int>;
    { a.set_display_width(int{}) } -> std::same_as<void>;
    { a.set_display_height(int{}) } -> std::same_as<void>;
    { b.get_fullscreen()} -> std::same_as<bool>;
    { a.set_fullscreen(bool{}) } -> std::same_as<void>;
    { b.get_running() } -> std::same_as<bool>;
    { a.set_running(bool{}) } -> std::same_as<void>;
};

constexpr const char* DEFAULT_TITLE = "Bubble Popper";

template<DisplayConfiguration T>
class Display {
    struct display_deleter {
        void operator()(ALLEGRO_DISPLAY* display) const;
    };
    struct event_deleter {
        void operator()(ALLEGRO_EVENT_QUEUE* queue) const;
    };

    std::unique_ptr<ALLEGRO_DISPLAY, display_deleter> m_display;
    std::unique_ptr<ALLEGRO_EVENT_QUEUE, event_deleter> m_event_queue;
    std::jthread m_event_handler;

    T& m_config;

    void handle_events();
    
    void create();
    void destroy();
public:
    Display(T& config);
    ~Display();
};