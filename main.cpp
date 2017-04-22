#include "lib/Game.h"
#include "lib/GUI.h"


int main() {
    Game::Init();
    GUI::Init();
    Game::Run();
    return EXIT_SUCCESS;
}