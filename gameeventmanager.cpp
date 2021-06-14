#include "gameeventmanager.h"

#include "utils.h"

GameEventManager* g_eventmanager = nullptr;

GameEventManager* GameEventManager::Singleton()
{
    if (g_eventmanager == nullptr) {
        g_eventmanager = (GameEventManager*)GetInterface("engine.dll", "GAMEEVENTSMANAGER002");
    }

    return g_eventmanager;
}

bool GameEventManager::AddListener(GameEventListener *listener, const char *name)
{
    // Bool specifies whether the context is for server or client, we only want client
    return GetVirtualFunction<bool(__thiscall*)(void*, GameEventListener*, const char*, bool)>(this, 3)(this, listener, name, false);
}

void GameEventManager::RemoveListener(GameEventListener *listener)
{
    GetVirtualFunction<void(__thiscall*)(void*, GameEventListener*)>(this, 5)(this, listener);
}
