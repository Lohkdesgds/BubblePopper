#include "setup.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_color.h>

AllegroSetup::~AllegroSetup() {
    uninstall();
}

void AllegroSetup::install() {
    if (m_installed) {
        return;
    }

    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_audio();
    al_init_acodec_addon();
    m_installed = true;
}

void AllegroSetup::uninstall() {
    if (!m_installed) {
        return;
    }
    al_uninstall_audio();
    al_shutdown_primitives_addon();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_image_addon();
    al_uninstall_system();
    m_installed = false;
}