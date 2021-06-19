#ifndef ARDUINOCOM_H
#define ARDUINOCOM_H

#include <Windows.h>

class ArduinoCom
{
public:
    ArduinoCom();
    ~ArduinoCom();

    bool tryConnect();

    bool setLightPower(float power);
private:
    HANDLE m_serialHandle;
};

#endif // ARDUINOCOM_H
