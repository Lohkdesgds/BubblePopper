#include "resources.h"

#include <allegro5/allegro_ttf.h>
#include <allegro5/file.h>
#include <allegro5/allegro_memfile.h>

#include <lunaris/console/console.h>

Resources::Resources() {
    m_embedded.push_back({"assets/EMprint-Regular.ttf", b::embed<"assets/EMprint-Regular.ttf">()});
    m_embedded.push_back({"assets/bubble.png", b::embed<"assets/bubble.png">()});

    Lunaris::cout << Lunaris::console::color::GREEN << "Resources initialized successfully!";
}

Resources::~Resources()
{
    Lunaris::cout << Lunaris::console::color::GRAY << "Resources destroyed.";
}


ALLEGRO_BITMAP* Resources::get_bitmap(const bitmap_identifier& name) {
    for (const auto& pair : m_bitmaps) {
        if (pair.first == name) {
            return pair.second;
        }
    }
    
    // Find the embedded file
    auto it = std::find_if(m_embedded.begin(), m_embedded.end(),
                           [&name](const auto& pair) { return pair.first == name; });
    
    if (it == m_embedded.end()) {
        Lunaris::cout << Lunaris::console::color::YELLOW << "Warning: Bitmap '" << name << "' not found in embedded resources! Nullptr returned.";
        return nullptr; // Bitmap not found
    }

    const auto& embedded_file = it->second;
    ALLEGRO_FILE* file = al_open_memfile(
        (void*)embedded_file.data(),
        static_cast<int64_t>(embedded_file.size()),
        "r"
    );

    // Create the bitmap from the embedded data
    ALLEGRO_BITMAP* bitmap = al_load_bitmap_f(file, nullptr);
    
    if (bitmap) {
        m_bitmaps.emplace_back(name, bitmap);
    }

    al_fclose(file);

    Lunaris::cout << Lunaris::console::color::GREEN << "New bitmap resource '" << name << "' created successfully from embedded resources.";
    return bitmap;
}

ALLEGRO_FONT* Resources::get_font(const font_identifier& identifier) {
    for (const auto& pair : m_fonts) {
        if (pair.first == identifier) {
            return pair.second;
        }
    }

    const auto& [font_name, font_size] = identifier;

    // Find the embedded file
    auto it = std::find_if(m_embedded.begin(), m_embedded.end(),
                           [&font_name](const auto& pair) { return pair.first == font_name; });

    if (it == m_embedded.end()) {
        Lunaris::cout << Lunaris::console::color::YELLOW << "Warning: Font '" << font_name << "' not found in embedded resources! Nullptr returned.";
        return nullptr; // Font not found
    }

    const auto& embedded_file = it->second;

    ALLEGRO_FILE* file = al_open_memfile(
        (void*)embedded_file.data(),
        static_cast<int64_t>(embedded_file.size()),
        "r"
    );

    // Create the font from the embedded data
    ALLEGRO_FONT* font = al_load_ttf_font_f(file, nullptr, font_size, 0);

    if (font) {
        m_fonts.emplace_back(identifier, font);
    }

    Lunaris::cout << Lunaris::console::color::GREEN << "New font resource '" << font_name << "' of size " << font_size << " created successfully from embedded resources.";

    return font;
}

void Resources::unload_bitmap(const bitmap_identifier& name) {
    auto it = std::remove_if(m_bitmaps.begin(), m_bitmaps.end(),
                             [&name](const auto& pair) { return pair.first == name; });
    if (it != m_bitmaps.end()) {
        al_destroy_bitmap(it->second);
        m_bitmaps.erase(it, m_bitmaps.end());
    }
}

void Resources::unload_font(const font_identifier& identifier) {
    auto it = std::remove_if(m_fonts.begin(), m_fonts.end(),
                             [&identifier](const auto& pair) { return pair.first == identifier; });
    if (it != m_fonts.end()) {
        al_destroy_font(it->second);
        m_fonts.erase(it, m_fonts.end());
    }
}
