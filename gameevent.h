#ifndef GAMEEVENT_H
#define GAMEEVENT_H

class GameEvent
{
public:
    const char* GetName();
    // keyName is the name of the event's attribute you want to retrieve
    int GetInt(const char* keyName, int defaultVal = 0);
    float GetFloat(const char* keyName, float defaultVal = 0.f);
    const char* GetString(const char* keyName, const char* defaultVal = "");
    void SetString(const char* keyName, const char* value);
private:
};

#endif // GAMEEVENT_H
