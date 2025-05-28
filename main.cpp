#include "game.h"
#include <ctime>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    Game().run();

    return 0;
}