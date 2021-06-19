#include "regkeyutils.h"

#define MAX_KEY_LENGTH 255

bool RegKeyUtils::GetRegKey(std::string_view name, HKEY& regKey)
{
    return RegOpenKeyExA(HKEY_LOCAL_MACHINE, name.data(), 0, KEY_READ, &regKey) == ERROR_SUCCESS;
}

bool RegKeyUtils::GetRegKeyInfo(HKEY regKey, RegKeyUtils::RegKeyInfo& info)
{
    info.key = regKey;
    return RegQueryInfoKeyA(regKey, NULL, NULL, NULL, &info.nSubKeys, &info.cbMaxSubKey, &info.cchMaxClass, NULL, NULL, NULL, NULL, NULL) == ERROR_SUCCESS;
}

bool RegKeyUtils::GetRegKeyString(HKEY regKey, std::string_view valueName, std::string& valueData)
{
    TCHAR szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);

    ULONG err = RegQueryValueExA(regKey, valueName.data(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);

    if (err != ERROR_SUCCESS) {
        return false;
    }

    valueData.resize(dwBufferSize);
    memcpy(valueData.data(), szBuffer, dwBufferSize);

    return true;
}

bool RegKeyUtils::GetSubKeyIndex(HKEY parentKey, DWORD index, RegKeyUtils::RegSubKeyInfo& info)
{
    // Get info about subkeys
    RegKeyUtils::RegKeyInfo parentInfo;
    if (!GetRegKeyInfo(parentKey, parentInfo) || parentInfo.nSubKeys < 1) {
        return false;
    }

    // Get 1st subkey
    TCHAR keyName[MAX_KEY_LENGTH];
    DWORD keyNameLen = MAX_KEY_LENGTH;
    if (RegEnumKeyExA(parentKey, index, keyName, &keyNameLen, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
        return false;
    }

    HKEY subkey;
    if (RegOpenKeyExA(parentKey, keyName, 0, KEY_READ, &subkey) != ERROR_SUCCESS) {
        return false;
    }

    info.name = keyName;

    return RegKeyUtils::GetRegKeyInfo(subkey, info.keyInfo);
}

std::vector<RegKeyUtils::RegSubKeyInfo> RegKeyUtils::GetSubKeys(HKEY parentKey)
{
    // Get info about subkeys
    RegKeyUtils::RegKeyInfo parentInfo;
    if (!GetRegKeyInfo(parentKey, parentInfo)) {
        return {};
    }

    std::vector<RegKeyUtils::RegSubKeyInfo> subkeys;

    // Iterate trough subkeys
    TCHAR keyName[MAX_KEY_LENGTH];
    DWORD keyNameLen;
    for (DWORD i = 0; i < parentInfo.nSubKeys; i++) {
        keyNameLen = MAX_KEY_LENGTH;
        if (RegEnumKeyExA(parentKey, i, keyName, &keyNameLen, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
            continue;
        }

        HKEY subkey;
        if (RegOpenKeyExA(parentKey, keyName, 0, KEY_READ, &subkey) != ERROR_SUCCESS) {
            continue;
        }

        RegKeyUtils::RegSubKeyInfo subkeyInfo;
        if (RegKeyUtils::GetRegKeyInfo(subkey, subkeyInfo.keyInfo)) {
            subkeyInfo.name = keyName;
            subkeys.push_back(std::move(subkeyInfo));
        }
    }

    return subkeys;
}
std::vector<RegKeyUtils::RegSubKeyInfo> RegKeyUtils::GetSubKeysThatStartsWith(HKEY parentKey, std::string_view match)
{
    // Get info about subkeys
    RegKeyUtils::RegKeyInfo parentInfo;
    if (!GetRegKeyInfo(parentKey, parentInfo)) {
        return {};
    }

    std::vector<RegKeyUtils::RegSubKeyInfo> subkeys;

    // Iterate trough subkeys
    TCHAR keyName[MAX_KEY_LENGTH];
    DWORD keyNameLen;
    for (DWORD i = 0; i < parentInfo.nSubKeys; i++) {
        keyNameLen = MAX_KEY_LENGTH;
        if (RegEnumKeyExA(parentKey, i, keyName, &keyNameLen, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
            continue;
        }

        if (keyNameLen < match.length()) {
            continue;
        }

        if (memcmp(match.data(), keyName, match.length()) != 0) {
            continue;
        }

        HKEY subkey;
        if (RegOpenKeyExA(parentKey, keyName, 0, KEY_READ, &subkey) != ERROR_SUCCESS) {
            continue;
        }

        RegKeyUtils::RegSubKeyInfo subkeyInfo;
        if (RegKeyUtils::GetRegKeyInfo(subkey, subkeyInfo.keyInfo)) {
            subkeyInfo.name = keyName;
            subkeys.push_back(std::move(subkeyInfo));
        }
    }

    return subkeys;
}
