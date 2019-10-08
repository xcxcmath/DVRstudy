//
// Created by bjk on 19. 9. 29..
//
#include <BasicHoloApp.hpp>

int main() {
    BasicHoloApp app(600, 600, 256, 256);

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what();
        return 1;
    }

    return 0;
}