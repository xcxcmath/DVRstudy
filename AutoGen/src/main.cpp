#include <AutoGenApp.hpp>

int main() {
    AutoGenApp app(400, 300);

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what();
        return 1;
    }

    return 0;
}