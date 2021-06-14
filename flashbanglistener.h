#ifndef FLASHBANGLISTENER_H
#define FLASHBANGLISTENER_H

#include "gameeventlistener.h"

class FlashbangListener : public GameEventListener
{
public:
    FlashbangListener() noexcept;
    void Remove() noexcept;
    void FireGameEvent(GameEvent* event);
};

#endif // FLASHBANGLISTENER_H
