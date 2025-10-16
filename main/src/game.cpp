#include <game.h>

#include <lunaris/console/console.h>

Game::Game()
    : m_resources()
    , m_config()
    , m_display(
        std::make_unique<Display>(m_config)
    )
{
    Lunaris::cout << Lunaris::console::color::GREEN << "Game initialized successfully!";
}

Game::~Game() {
    m_config.set_fullscreen(m_display->is_fullscreen());
    
    Lunaris::cout << Lunaris::console::color::GRAY << "Game destroyed.";
}

void Game::run() {
    // Main game loop would go here

    const auto last_time = al_get_time();

    while(*m_display && al_get_time() - last_time < 10.0) {
        ALLEGRO_COLOR rainbow_by_time = al_map_rgb(
            120 + cos(al_get_time() * 2) * 100,
            120 + cos(al_get_time() * 1.3 + 2) * 100,
            120 + cos(al_get_time() * 2.7 + 4) * 100
        );

        al_clear_to_color(rainbow_by_time);
        al_flip_display();
    }
}