#include <Windows.h>

#include <chrono>
#include <thread>
#include <iostream>

#include "csgo.h"

template <typename T>
T readMemory(ptrdiff_t ptr)
{
    return *(T*)ptr;
}

float lastStrength = 0;
float currentStrength = 0;
std::chrono::high_resolution_clock::time_point lastChange;


void printStrength()
{
    std::cout << "Flash expires in " << currentStrength << " seconds" << std::endl;
}

void handleFlashStrength(float strength)
{
    strength *= 0.7f; // Asjust for timing inaccuracy

    if (strength != lastStrength) {
        if (strength > currentStrength) {
            currentStrength = strength;
            printStrength();
        }
        lastStrength = strength;
        lastChange = std::chrono::high_resolution_clock::now();
    }

    if (currentStrength > 0) {
        auto now = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastChange).count();

        if (diff >= 25)
        {
            currentStrength -= 0.025f;

            if (currentStrength < 0) {
                currentStrength = 0;
            }

            printStrength();
            lastChange = now;
        }
    }
}

DWORD WINAPI UwU(HMODULE hModule)
{
    AllocConsole();

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "FlashbangEnhancer loaded!" << std::endl;

    DWORD moduleBase =(DWORD)GetModuleHandle("client.dll");

    lastChange = std::chrono::high_resolution_clock::now();

    while (!GetAsyncKeyState(VK_F1)) {

        DWORD localPlayer = readMemory<DWORD>(moduleBase + offsets::dwLocalPlayer);

        if (localPlayer != 0) {
            float strength = readMemory<float>(localPlayer + offsets::m_flFlashDuration);
            handleFlashStrength(strength);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "Unloading FlashbangEnhancer" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    fclose(f);

    FreeConsole();

    FreeLibraryAndExitThread(hModule, 0);

    CloseHandle(hModule);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdReason, LPVOID)
{

    switch (fdReason) {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)UwU, hModule, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    default:
        return FALSE;
    }

    return TRUE;
}
