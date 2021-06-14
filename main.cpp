#include <cstdlib>

#include "gameeventmanager.h"
#include "flashbanglistener.h"

int main(int argc, char** argv)
{
    FlashbangListener listener;

    GameEventManager::Singleton()->AddListener(&listener, "player_blind");

    return EXIT_SUCCESS;
}
