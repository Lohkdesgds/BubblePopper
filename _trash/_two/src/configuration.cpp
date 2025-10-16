#include "configuration.h"

Configuration::Configuration() {
    m_saveable["display_width"] = 800;
    m_saveable["display_height"] = 600;
    m_saveable["fullscreen"] = false;

    m_non_saveable["running"] = true;
}

Configuration::~Configuration() {

}

int Configuration::get_display_width() const {
    return m_saveable.value("display_width", 800);
}
int Configuration::get_display_height() const {
    return m_saveable.value("display_height", 600);
}
void Configuration::set_display_width(int width) {
    m_saveable["display_width"] = width;
}
void Configuration::set_display_height(int height) {
    m_saveable["display_height"] = height;
}
bool Configuration::get_fullscreen() const {
    return m_saveable.value("fullscreen", false);
}
void Configuration::set_fullscreen(bool fullscreen) {
    m_saveable["fullscreen"] = fullscreen;
}
void Configuration::set_running(bool running) {
    m_non_saveable["running"] = running;
}
bool Configuration::get_running() const {
    return m_non_saveable.value("running", true);
}
