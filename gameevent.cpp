#include "gameevent.h"

#include "utils.h"

const char *GameEvent::GetName()
{
    return GetVirtualFunction<const char*(__thiscall*) (void*)>(this, 1)(this);
}

int GameEvent::GetInt(const char *keyName, int defaultVal)
{
    return GetVirtualFunction<int(__thiscall*) (void*, const char*, int)>(this, 6)(this, keyName, defaultVal);
}

float GameEvent::GetFloat(const char *keyName, float defaultVal)
{
    return GetVirtualFunction<float(__thiscall*) (void*, const char*, float)>(this, 8)(this, keyName, defaultVal);
}

const char *GameEvent::GetString(const char *keyName, const char *defaultVal)
{
    return GetVirtualFunction<const char* (__thiscall*)(void*, const char*, const char*)>(this, 9)(this, keyName, defaultVal);
}

void GameEvent::SetString(const char *keyName, const char *value)
{
    GetVirtualFunction<void(__thiscall*)(void*, const char*, const char*)>(this, 16)(this, keyName, value);
}
