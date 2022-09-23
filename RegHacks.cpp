#include <cstdio>
#include <string>
#include <thread>
#include <cstddef>
#include <cstdint>
#include <filesystem>

#include "hacks/all.h"
#include "HCl/All.hpp"

using namespace std::literals;
namespace fs = std::filesystem;

HTextArea logs;

const char* reghacks_ascii_art = "\r\n"
"                             WELCOME TO REG HACKS v1.0\r\n"
"             Use the Provided Buttons to Enable/Disable Hacks.\r\n";

void clear()
{
    logs.setText("");
}

void print(const auto& ...args)
{
    std::stringstream ss;
    (ss << ... << args);
    logs.setText(logs.getText()+ss.str());
}

void println(const auto& ...args)
{
    std::stringstream ss;
    (ss << ... << args) << "\r\n";
    logs.setText(logs.getText()+ss.str());
};

enum class result : uint8_t
{
    FALIURE = 0,
    SUCCESS = 1
};

void playSound(result r)
{
    if (r == result::SUCCESS)
        PlaySoundA("C:/Windows/Media/chimes.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
    else
        PlaySoundA("C:/Windows/Media/chord.wav" , NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
}

void cwd2ExePath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    auto exepath = std::string(buffer);
    auto dirpath = exepath.substr(0, exepath.find_last_of("\\/"));
    SetCurrentDirectoryA(dirpath.c_str());
}

void cwd2TempPath()
{
    SetCurrentDirectoryW(fs::temp_directory_path().wstring().c_str());
}

bool iAmAdmin()
{
    bool IsElevated = false;
    HANDLE hToken = NULL;
    TOKEN_ELEVATION elevation;
    DWORD dwSize;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
        goto Cleanup; // if Failed, we treat as False

    if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
        goto Cleanup; // if Failed, we treat as False

    IsElevated = elevation.TokenIsElevated;

Cleanup:
    if (hToken)
    {
        CloseHandle(hToken);
        hToken = NULL;
    }
    return IsElevated; 
}

bool use_hack_file(uint8_t id)
{
    auto filename = std::to_string(id) + ".reg";
    FILE* f = fopen(filename.c_str(), "wb");
    if (!f) return false;
    fwrite(rhe_map[id].data, 1, rhe_map[id].size, f);
    fclose(f);
    system(("reg import "+filename).c_str());
    DeleteFileA(filename.c_str());
    return true;
}

void button_boilerplate(uint8_t id, const char* desc, bool reboot_required=true)
{
    clear(); println(); print(desc);
    if (use_hack_file(id))
    {
        println("Done!"); println();
        if (reboot_required)
            print("Changes will take Effect after a Reboot!");
        playSound(result::SUCCESS);
    }
    else
    {
        println("Error!");
        playSound(result::FALIURE);
    }
}

void disable_windows_defender()
{
    HKEY key, new_key;
    DWORD disable = 1;
    if (ERROR_SUCCESS == RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, KEY_ALL_ACCESS, &key))
    {
        RegSetValueExA(key,     "DisableAntiSpyware", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
        RegCreateKeyExA(key,    "Real-Time Protection", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &new_key, 0);
        RegSetValueExA(new_key, "DisableRealtimeMonitoring", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
        RegSetValueExA(new_key, "DisableBehaviorMonitoring", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
        RegSetValueExA(new_key, "DisableScanOnRealtimeEnable", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
        RegSetValueExA(new_key, "DisableOnAccessProtection", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
        RegSetValueExA(new_key, "DisableIOAVProtection", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
        
        RegCloseKey(key);
        RegCloseKey(new_key);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    cwd2TempPath();
    ReportHINSTANCE(hInstance);
    
    if (!iAmAdmin())
    {
        MessageBox(NULL, L"Please run this program as Administrator.", L"Runtime Error", MB_OK);
        return 1;
    }

    HFrame frame = HFrame("RegHacks v1.0 for Windows 10/11");
    frame.setSize(610, 450);
    frame.setDefaultCloseOperation(frame.EXIT_ON_CLOSE);
    
    logs = HTextArea("logs");
    logs.setSize(480, 100);
    logs.setLocation(60, 10);
    logs.setText(reghacks_ascii_art);
    
    HButton b1("Enable Windows Photo Viewer");
    b1.setSize(240, 70);
    b1.setLocation(50, 120);
    b1.addActionListener([](ActionEvent) -> int
    {
        button_boilerplate(1, "Enabling Windows Photo Viewer...");
        return 0;
    });
    
    HButton b2("Disable Bing in Search Bar");
    b2.setSize(240, 70);
    b2.setLocation(310, 120);
    b2.addActionListener([](ActionEvent) -> int
    {
        button_boilerplate(2, "Disabling Bing in Search Bar...");
        return 0;
    });

    HButton b3("Enable Direct Sign In Screen");
    b3.setSize(240, 70);
    b3.setLocation(50, 210);
    b3.addActionListener([](ActionEvent) -> int
    {
        button_boilerplate(3, "Enabling Direct Sign In Screen...");
        return 0;
    });

    HButton b4("Disable Ads and Telemetery");
    b4.setSize(240, 70);
    b4.setLocation(310, 210);
    b4.addActionListener([](ActionEvent) -> int
    {
        button_boilerplate(4, "Disabling Ads and Telemetery...");
        return 0;
    });

    HButton b5("Disable Windows Updates");
    b5.setSize(240, 70);
    b5.setLocation(50, 300);
    b5.addActionListener([](ActionEvent) -> int
    {
        button_boilerplate(5, "Disabling Windows Updates...");
        return 0;
    });

    HButton b6("Disable Windows Defender");
    b6.setSize(240, 70);
    b6.setLocation(310, 300);
    b6.addActionListener([](ActionEvent) -> int
    {
        println("Disabling Windows Defender...");
        disable_windows_defender();
        println("Done!"); println();
        print("Changes will take Effect after a Reboot!");
        playSound(result::SUCCESS);
        return 0;
    });

    frame.add(&logs);
    frame.add(&b1);
    frame.add(&b2);
    frame.add(&b3);
    frame.add(&b4);
    frame.add(&b5);
    frame.add(&b6);
    frame.setVisible(true);

    std::thread([]()
    {
        std::this_thread::sleep_for(30s);
        logs.setText(reghacks_ascii_art);
    })
    .detach();

    WaitAll();
    return 0;
}