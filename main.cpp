#include "lib/Game.h"
#include "lib/GUI.h"


int main() {
    Game::Init();
    GUI::Init();
    GL::Init();
    Game::Run();
    GL::Destroy();
    GUI::Destroy();
    return EXIT_SUCCESS;
}