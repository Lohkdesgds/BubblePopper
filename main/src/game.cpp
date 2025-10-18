#include <game.h>

#include <lunaris/console/console.h>

Game::Game()
    : m_resources()
    , m_config()
    , m_display(
        std::make_unique<Display>(m_config)
    )
{
    m_event_queue_wrapper.register_source(*m_display);
    m_event_queue_wrapper.register_source(al_get_keyboard_event_source());
    m_event_queue_wrapper.register_source(al_get_mouse_event_source());

    Lunaris::cout << Lunaris::console::color::GREEN << "Game initialized successfully!";
}

Game::~Game() {
    m_config.set_fullscreen(m_display->is_fullscreen());
    
    Lunaris::cout << Lunaris::console::color::GRAY << "Game destroyed.";
}

bool Game::think() {
    ALLEGRO_EVENT event;
    if (!m_event_queue_wrapper.get_event(&event))
        return false;

    switch (event.type) {
    case ALLEGRO_EVENT_KEY_DOWN:
        switch(event.keyboard.keycode) {
        case ALLEGRO_KEY_F11:
            Lunaris::cout << Lunaris::console::color::YELLOW << "Toggled fullscreen mode: "
                << (m_display->toggle_fullscreen() ? "ON" : "OFF");
            break;
        case ALLEGRO_KEY_F3:
            m_config.set_debug_mode(!m_config.is_debug_mode());
            Lunaris::cout << Lunaris::console::color::YELLOW << "Toggled debug mode: "
                << (m_config.is_debug_mode() ? "ON" : "OFF");
            break;
        }
        break;
    default:
        break;
    }

    return true;
}

void Game::draw() {
    // Main game loop would go here

    ALLEGRO_COLOR rainbow_by_time = al_map_rgb(
        120 + cos(al_get_time() * 2) * 100,
        120 + cos(al_get_time() * 1.3 + 2) * 100,
        120 + cos(al_get_time() * 2.7 + 4) * 100
    );

    al_clear_to_color(rainbow_by_time);


    if (m_config.is_debug_mode()) {
        ALLEGRO_FONT* font = m_resources.get_font({"assets/EMprint-Regular.ttf", 14});
        al_draw_multiline_textf(
            font,
            al_map_rgb(255, 255, 255),
            10, 10,
            m_display->get_width() - 20,
            20, 0,
            "FPS: %u\nFrame time: %.6lf ms\nResolution: %dx%d\nFullscreen: %s\nVSync: %s",
            m_display->get_fps(),
            m_display->get_frame_time_ms(),
            m_display->get_width(),
            m_display->get_height(),
            m_display->is_fullscreen() ? "Yes" : "No",
            m_config.is_vsync() ? "Yes" : "No"
        );
    }
    
    m_display->flip();
}

Game::operator bool() const {
    return static_cast<bool>(*m_display);
}