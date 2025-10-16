#pragma once

class AllegroSetup {
    bool m_installed = false;
public:
    ~AllegroSetup();

    void install();
    void uninstall();
};