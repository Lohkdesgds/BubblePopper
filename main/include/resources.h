#pragma once

#include <battery/embed.hpp>
#include <vector>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class Resources {
public:
    using bitmap_identifier = std::string;
    using font_identifier = std::tuple<std::string, int>; // name, size
public:
    Resources();

    ALLEGRO_BITMAP* get_bitmap(const bitmap_identifier& name);
    ALLEGRO_FONT* get_font(const font_identifier& name);

    void unload_bitmap(const bitmap_identifier& name);
    void unload_font(const font_identifier& name);
private:
    std::vector<std::pair<std::string, b::EmbedInternal::EmbeddedFile>> m_embedded;
    std::vector<std::pair<bitmap_identifier, ALLEGRO_BITMAP*>> m_bitmaps;
    std::vector<std::pair<font_identifier, ALLEGRO_FONT*>> m_fonts;
};