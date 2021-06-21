#include "arduinocom.h"

#include "regkeyutils.h"
#include <string>
#include <iostream>

ArduinoCom::ArduinoCom()
    : m_serialHandle(INVALID_HANDLE_VALUE)
    , m_portname()
    , m_friendlyname()
{
}

ArduinoCom::~ArduinoCom()
{
    disconnect();
}

bool ArduinoCom::tryConnect()
{
    // Open registry key for usb devices
    HKEY devicesKey;
    if (!RegKeyUtils::GetRegKey("SYSTEM\\CurrentControlSet\\Enum\\USB", devicesKey)) {
        return false;
    }

    auto subkeys = RegKeyUtils::GetSubKeysThatStartsWith(devicesKey, "VID_2341&PID_");
    for (auto subkey : subkeys) {
        disconnect();

        RegKeyUtils::RegSubKeyInfo info;

        // Get 1st folder
        if (!RegKeyUtils::GetSubKeyIndex(subkey.keyInfo.key, 0, info)) {
            continue;
        }

        // Get friendly name of port
        if (!RegKeyUtils::GetRegKeyString(info.keyInfo.key, "FriendlyName", m_friendlyname)) {
            continue;
        }

        // Get 2nd folder
        if (!RegKeyUtils::GetSubKeyIndex(info.keyInfo.key, 0, info)) {
            continue;
        }

        // Get portname
        if (!RegKeyUtils::GetRegKeyString(info.keyInfo.key, "PortName", m_portname)) {
            continue;
        }

        HANDLE serialHandle = CreateFileA(("\\\\.\\" + m_portname).data(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if (serialHandle == INVALID_HANDLE_VALUE) {
            continue;
        }

        // Do some basic settings
        DCB serialParams{};
        serialParams.DCBlength = sizeof(serialParams);
        if (GetCommState(serialHandle, &serialParams) == FALSE) {
            continue;
        }

        serialParams.BaudRate = CBR_115200;
        serialParams.ByteSize = 8;
        serialParams.StopBits = ONESTOPBIT;
        serialParams.Parity   = NOPARITY;
        if (SetCommState(serialHandle, &serialParams) == FALSE) {
            continue;
        }

        // Set timeouts
        COMMTIMEOUTS timeout{};
        timeout.ReadIntervalTimeout = 50;
        timeout.ReadTotalTimeoutConstant = 50;
        timeout.ReadTotalTimeoutMultiplier = 10;
        timeout.WriteTotalTimeoutConstant = 50;
        timeout.WriteTotalTimeoutMultiplier = 10;
        if (SetCommTimeouts(serialHandle, &timeout) == FALSE) {
            continue;
        }

        m_serialHandle = serialHandle;

        return true;
    }

    disconnect();
    return false;
}

void ArduinoCom::disconnect()
{
    CloseHandle(m_serialHandle);
    m_serialHandle = INVALID_HANDLE_VALUE;
    m_friendlyname.clear();
    m_portname.clear();
}

bool ArduinoCom::isConnected() const noexcept
{
    return m_serialHandle != INVALID_HANDLE_VALUE;
}

std::string ArduinoCom::portname() const
{
    return m_portname;
}

std::string ArduinoCom::friendlyname() const
{
    return m_friendlyname;
}

bool ArduinoCom::setLightPower(float power)
{
    float fval = power;

    fval /= 3.f;
    fval *= 255;
    fval  = fval > 255.f ? 255.f : fval;

    std::uint8_t command[3]{ 0xAA, 0x00, 0xCC };
    command[1] = (std::uint8_t)fval;

    if (WriteFile(m_serialHandle, command, 3, NULL, NULL) == FALSE) {
        if (GetLastError() == ERROR_BROKEN_PIPE) {
            disconnect();
        }
        return false;
    }

    return true;
}
