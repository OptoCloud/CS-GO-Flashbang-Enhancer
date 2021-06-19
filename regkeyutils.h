#ifndef REGKEYUTILS_H
#define REGKEYUTILS_H

#include <Windows.h>
#include <vector>
#include <string_view>

namespace RegKeyUtils {
struct RegKeyInfo {
    HKEY key;
    DWORD nSubKeys = 0;
    DWORD cbMaxSubKey;
    DWORD cchMaxClass;
    std::string name;
};
struct RegSubKeyInfo {
    RegKeyInfo keyInfo;
    std::string name;
};
struct RegKeyValue {
    RegKeyInfo keyInfo;
    std::string name;
};
bool GetRegKey(std::string_view name, HKEY& regKey);
bool GetRegKeyInfo(HKEY regKey, RegKeyInfo& info);
bool GetRegKeyString(HKEY regKey, std::string_view valueName, std::string& valueData);
bool GetSubKeyIndex(HKEY parentKey, DWORD index, RegSubKeyInfo& info);
std::vector<RegSubKeyInfo> GetSubKeys(HKEY parentKey);
std::vector<RegSubKeyInfo> GetSubKeysThatStartsWith(HKEY parentKey, std::string_view match);
}

#endif // REGKEYUTILS_H
