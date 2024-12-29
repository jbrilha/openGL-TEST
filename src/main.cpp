#include "game.hpp"

int main(int argc, char *argv[]) {
    (void)argc, (void)argv; // to silence the unused warning

    Game game;

    game.init();

    game.run();

    return EXIT_SUCCESS;
}
