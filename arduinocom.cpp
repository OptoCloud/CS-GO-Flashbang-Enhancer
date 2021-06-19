#include "arduinocom.h"

#include "regkeyutils.h"
#include <string>
#include <iostream>

struct RegKeyInfo {
    DWORD nSubKeys = 0;
    DWORD cbMaxSubKey;
    DWORD cchMaxClass;
};
bool GetRegKeyInfo(HKEY regKey, RegKeyInfo& info) {
    return RegQueryInfoKeyA(regKey, NULL, NULL, NULL, &info.nSubKeys, &info.cbMaxSubKey, &info.cchMaxClass, NULL, NULL, NULL, NULL, NULL) == ERROR_SUCCESS;
}

ArduinoCom::ArduinoCom()
{
}

ArduinoCom::~ArduinoCom()
{
    CloseHandle(m_serialHandle);
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
        RegKeyUtils::RegSubKeyInfo info;

        // Get 1st folder
        if (!RegKeyUtils::GetSubKeyIndex(subkey.keyInfo.key, 0, info)) {
            continue;
        }

        // Get 2nd folder
        if (!RegKeyUtils::GetSubKeyIndex(info.keyInfo.key, 0, info)) {
            continue;
        }

        // Get portname
        std::string portname;
        if (!RegKeyUtils::GetRegKeyString(info.keyInfo.key, "PortName", portname)) {
            continue;
        }

        HANDLE serialHandle = CreateFileA(("\\\\.\\" + portname).data(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if (serialHandle == INVALID_HANDLE_VALUE) {
            continue;
        }

        std::cout << "Connected to " << portname << std::endl;

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

    return false;
}

bool ArduinoCom::setLightPower(float power)
{
    try {
        float fval = power;

        fval /= 3.f;
        fval *= 255;
        fval  = fval > 255.f ? 255.f : fval;

        std::uint8_t command[3]{ 0xAA, 0x00, 0xCC };
        command[1] = (std::uint8_t)fval;

        if (WriteFile(m_serialHandle, command, 3, NULL, NULL) == FALSE) {
            return false;
        }
    }  catch (...) { return false; }

    return true;
}
