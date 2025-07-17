#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_color.h>

int main()
{
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_audio();
    al_init_acodec_addon();

    std::cout << "Allegro initialized successfully!" << std::endl;

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display) exit(EXIT_FAILURE);
    al_set_window_title(display, "Bubble Popper - Loading...");

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    if (!event_queue) {
        al_destroy_display(display);
        exit(EXIT_FAILURE);
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_set_window_title(display, "Bubble Popper");
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    while(1) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
    }

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}