#ifndef GAMEEVENTMANAGER_H
#define GAMEEVENTMANAGER_H

class GameEventListener;

// The game's interface
class GameEventManager
{
public:
    static GameEventManager* Singleton();

    bool AddListener(GameEventListener* listener, const char* name);
    void RemoveListener(GameEventListener* listener);
};

#endif // GAMEEVENTMANAGER_H

