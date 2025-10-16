#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <iostream>

#include <lunaris/console/console.h>

#include <game.h>

using namespace Lunaris;

int start_allegro();

int main(int argc, char** argv) {
    cout << "Loading Allegro...";

    // If any of the args is "-debug", enable verbose build
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-debug") {
            al_set_config_value(al_get_system_config(), "system", "log", "allegro.log");
        }
    }

    if (int err = start_allegro())
        return err;

    cout << console::color::GREEN << " Allegro loaded successfully! Starting game...";

    const auto test = al_get_standard_path(ALLEGRO_USER_SETTINGS_PATH); // ALLEGRO_USER_DATA_PATH, ALLEGRO_USER_SETTINGS_PATH
    cout << console::color::AQUA << al_path_cstr(test, '/');



    Game game;
    game.run();
}

int start_allegro() {
    if (!al_init()) {
        cout << console::color::RED << "Failed to initialize Allegro!";
        return -1;
    }
    
    if (!al_init_primitives_addon()) {
        cout << console::color::RED << "Failed to initialize Allegro Primitives Addon!";
        return -2;
    }
    
    if (!al_init_image_addon()) {
        cout << console::color::RED << "Failed to initialize Allegro Image Addon!";
        return -3;
    }

    if (!al_init_font_addon()) {
        cout << console::color::RED << "Failed to initialize Allegro Font Addon!";
        return -4;
    }
    
    if (!al_init_ttf_addon()) {
        cout << console::color::RED << "Failed to initialize Allegro TTF Addon!";
        return -5;
    }

    if (!al_install_audio()) {
        cout << console::color::RED << "Failed to initialize Allegro Audio!";
        return -6;
    }

    if (!al_init_acodec_addon()) {
        cout << console::color::RED << "Failed to initialize Allegro Audio Codec Addon!";
        return -7;
    }

    if (!al_reserve_samples(16)) {
        cout << console::color::RED << "Failed to reserve Allegro Audio Samples!";
        return -8;
    }

    if (!al_install_keyboard()) {
        cout << console::color::RED << "Failed to initialize Allegro Keyboard!";
        return -9;
    }
    
    if (!al_install_mouse()) {
        cout << console::color::RED << "Failed to initialize Allegro Mouse!";
        return -10;
    }

    return 0;
}