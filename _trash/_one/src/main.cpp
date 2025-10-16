#include <iostream>

#include "setup.h"
#include "display.h"


#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_color.h>

#include <concepts>

template<typename T>
concept Allocalable = requires (T t) {
    { t.create() } -> std::same_as<void>;
    { t.destroy() } -> std::same_as<void>;
};


void make_create(Allocalable auto& obj) {
    obj.create();
}


int main()
{
    AllegroSetup allegroSetup;
    allegroSetup.install();

    std::unique_ptr<Display> display = std::make_unique<Display>();

    make_create(*display);

    std::cout << "Allegro initialized successfully!" << std::endl;

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    if (!event_queue) {
        al_destroy_display(display->get());
        exit(EXIT_FAILURE);
    }

    al_register_event_source(event_queue, al_get_display_event_source(display->get()));

    display->set_title("Bubble Popper");

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

    return 0;
}