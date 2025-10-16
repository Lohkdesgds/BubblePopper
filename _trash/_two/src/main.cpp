#include <iostream>

#include <configuration.h>
#include <display.h>

int main() {
    Configuration config;
    Display<Configuration> display(config);

    while(config.get_running()) std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return 0;
}