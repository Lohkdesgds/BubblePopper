#include <game.h>

Game::Game()
    : m_resources()
    , m_config()
    , m_display(std::make_unique<Display>(m_config.get_screen_width(), m_config.get_screen_height(), m_config.is_fullscreen(), m_config.is_vsync()))
{
}

Game::~Game() {
    m_config.set_screen_width(m_display->get_width());
    m_config.set_screen_height(m_display->get_height());
    m_config.set_fullscreen(m_display->is_fullscreen());
}

void Game::run() {
    // Main game loop would go here

    while(*m_display) {
        ALLEGRO_COLOR rainbow_by_time = al_map_rgb(
            120 + cos(al_get_time() * 2) * 100,
            120 + cos(al_get_time() * 1.3 + 2) * 100,
            120 + cos(al_get_time() * 2.7 + 4) * 100
        );

        al_clear_to_color(rainbow_by_time);
        al_flip_display();
    }
}