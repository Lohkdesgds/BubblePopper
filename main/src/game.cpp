#include <game.h>

#include <lunaris/console/console.h>

Game::Game()
    : m_resources(),
    m_config(),
    m_display(
        std::make_unique<Display>(m_config)
    ),
    m_table(m_resources, 26, 26)
    //bubble(m_resources.get_bitmap("assets/bubble.png"), 800, 800, BubbleColors::get(BubbleColors::Color::AQUA))
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
        case ALLEGRO_KEY_R:
            m_table.random_fill();
            break;
        //case ALLEGRO_KEY_W:
        //    bubble.move_to(bubble.get_x(), bubble.get_y() - 200);
        //    break;
        //case ALLEGRO_KEY_A:
        //    bubble.move_to(bubble.get_x() - 200, bubble.get_y());
        //    break;
        //case ALLEGRO_KEY_S:
        //    bubble.move_to(bubble.get_x(), bubble.get_y() + 200);
        //    break;
        //case ALLEGRO_KEY_D:
        //    bubble.move_to(bubble.get_x() + 200, bubble.get_y());
        //    break;

        }
        break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        {
            float mx = static_cast<float>(event.mouse.x);
            float my = static_cast<float>(event.mouse.y);

            m_display->transform_mouse_coords(mx, my);
            m_table.mouse_click_at(mx, my);

            //printf("%.1f, %.1f\n", mx, my);
            
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

    //bubble.draw(*m_display);
    m_table.draw_all(*m_display);


    if (m_config.is_debug_mode()) {
        ALLEGRO_FONT* font = m_resources.get_font({"assets/EMprint-Regular.ttf", 64});
        al_draw_multiline_textf(
            font,
            al_map_rgb(255, 255, 255),
            10 - m_display->get_draw_width(), 10 - m_display->get_draw_height(),
            m_display->get_draw_width() - 20,
            56, 0,
            "FPS: %u\nFrame time: %.6lf ms\nResolution: %dx%d\nDraw canvas: %.2fx%.2f zoom=%.5f\nFullscreen [F11]: %s\nVSync: %s",
            m_display->get_fps(),
            m_display->get_frame_time_ms(),
            m_display->get_width(), m_display->get_height(),
            m_display->get_draw_width(), m_display->get_draw_height(), m_display->get_draw_scale(),
            m_display->is_fullscreen() ? "Yes" : "No",
            m_config.is_vsync() ? "Yes" : "No"
        );
    }
    
    m_display->flip();
}

Game::operator bool() const {
    return static_cast<bool>(*m_display);
}