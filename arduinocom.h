#ifndef ARDUINOCOM_H
#define ARDUINOCOM_H

#include <Windows.h>
#include <string>

class ArduinoCom
{
public:
    ArduinoCom();
    ~ArduinoCom();

    bool tryConnect();
    void disconnect();

    bool isConnected() const noexcept;
    std::string portname() const;
    std::string friendlyname() const;

    bool setLightPower(float power);
private:
    HANDLE m_serialHandle;
    std::string m_portname;
    std::string m_friendlyname;
};

#endif // ARDUINOCOM_H
