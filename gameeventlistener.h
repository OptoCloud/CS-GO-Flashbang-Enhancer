#ifndef GAMEEVENTLISTENER_H
#define GAMEEVENTLISTENER_H

class GameEvent;

class GameEventListener {
public:
    virtual ~GameEventListener() {}
    virtual void FireGameEvent(GameEvent* event) = 0;
    // Tells the server whether the event should be handled by this class instance. 42 if we want to handle it (we do)
    virtual int IndicateEventHandling() { return 42; }
private:
};

#endif // GAMEEVENTLISTENER_H
