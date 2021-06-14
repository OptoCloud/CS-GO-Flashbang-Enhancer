#include "flashbanglistener.h"

#include "gameevent.h"
#include "gameeventmanager.h"

#include <iostream>
#include <cstring>

FlashbangListener::FlashbangListener() noexcept
{

}

void FlashbangListener::Remove() noexcept
{
    GameEventManager::Singleton()->RemoveListener(this);
}

void FlashbangListener::FireGameEvent(GameEvent* event)
{
    // Check which event was just triggered. You'll only need this if you listen to multiple events
    if (strncmp(event->GetName(), "player_blind", sizeof("player_blind")) == 0)
    {
        // Find out how much damage was done
        float duration = event->GetFloat("blind_duration");
        std::cout << "Player blinded for " << duration << " seconds" << std::endl;
    }
}
