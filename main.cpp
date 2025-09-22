#define NOMINMAX
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <shlobj.h>
#include <filesystem>
#include <limits>
#include <urlmon.h>

#pragma comment(lib, "urlmon.lib")

namespace fs = std::filesystem;

void enableAnsiColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void setColorAttr(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void drawHeader() {
    std::cout << "\033[96m"
        R"(
                                        _____           ____        _ _ _                             
                                       | ____|________ | __ )  ___ (_|_|_)                            
                                       |  _| |_  /_  / |  _ \ / _ \| | | |                            
                                       | |___ / / / /  | |_) | (_) | | | |                            
                        _        _   _ |_____/___/___|_|____/ \___/|_|_|_|          _   _             
                       / \   ___| |_(_)_   _____   / ___|___  _ __  _ __   ___  ___| |_(_) ___  _ __  
                      / _ \ / __| __| \ \ / / _ \ | |   / _ \| '_ \| '_ \ / _ \/ __| __| |/ _ \| '_ \ 
                     / ___ \ (__| |_| |\ V /  __/ | |__| (_) | | | | | | |  __/ (__| |_| | (_) | | | |
                    /_/   \_\___|\__|_| \_/ \___|  \____\___/|_| |_|_| |_|\___|\___|\__|_|\___/|_| |_|
                                                  |  ___(_)_  __                                      
                                                  | |_  | \ \/ /                                      
                                                  |  _| | |>  <                                       
                                                  |_|   |_/_/\_\                                      
)"
<< "\033[0m\n\n";
}

void pauseAndReturn() {
    std::cout << "\n\n\033[90mPress any key to return to the main menu...\033[0m";
    _getch();
}

void clearScreen() {
    system("cls");
}

std::string getLocalAppDataPath() {
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) {
        return std::string(path);
    }
    return "";
}

bool ensureParentDirExists(const std::string& filePath) {
    try {
        fs::path p(filePath);
        fs::path parent = p.parent_path();
        if (!parent.empty() && !fs::exists(parent)) {
            fs::create_directories(parent);
        }
        return true;
    }
    catch (...) {
        return false;
    }
}

bool downloadFile(const std::string& url, const std::string& outputPath) {
    ensureParentDirExists(outputPath);
    HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), outputPath.c_str(), 0, NULL);
    if (hr == S_OK) {
        std::cout << "\033[92mSuccessfully Downloaded.\033[0m\n";
        return true;
    }
    else {
        std::cout << "\033[93mError While Downloading (HRESULT: 0x" << std::hex << hr << std::dec << "). Trying curl...\033[0m\n";
    }
    std::string command = "curl -sL -o \"" + outputPath + "\" \"" + url + "\"";
    int result = system(command.c_str());
    if (result == 0) {
        std::cout << "\033[92mDownloaded via curl.\033[0m\n";
        return true;
    }
    else {
        std::cout << "\033[91mcurl failed (return code " << result << ").\033[0m\n";
    }
    return false;
}

bool extractZip(const std::string& zipPath, const std::string& extractTo) {
    try {
        if (!fs::exists(extractTo)) fs::create_directories(extractTo);
    }
    catch (...) {}
    std::string command = "powershell -NoProfile -Command \"Expand-Archive -Path \\\"" + zipPath + "\\\" -DestinationPath \\\"" + extractTo + "\\\" -Force\"";
    int result = system(command.c_str());
    return result == 0;
}

void showGuide() {
    clearScreen();
    std::cout << "\033[93m==================== HELP & GUIDE ====================\033[0m\n\n";
    std::cout << "This program helps you fix the BO3 error related to active connection.\n";
    std::cout << "Use the menu options to either view a guide, perform automatic fixes,\n";
    std::cout << "or get help and join our Discord.\n\n";
    std::cout << "\033[90m--------------------------------------------------------\033[0m\n";
    std::cout << "\033[93m1.\033[0m Guide:\n   Opens a webpage with detailed instructions.\n";
    std::cout << "\033[93m2.\033[0m Fix:\n   Downloads and extracts necessary files automatically.\n";
    std::cout << "\033[93m3.\033[0m Help:\n   Shows this guide and offers to join Discord.\n";
    std::cout << "\033[93m4.\033[0m Exit:\n   Close the program.\n\n";
    pauseAndReturn();
}

void openGuideURL() {
    std::cout << "\033[93mWould you like to open the guide website? (Y/N): \033[0m";
    char answer = _getch();
    if (answer == 'Y' || answer == 'y') {
        ShellExecuteA(NULL, "open", "https://forum.ezz.lol/topic/10/bo3-error-active-connection", NULL, NULL, SW_SHOWNORMAL);
    }
}

void showSourceCode() {
    clearScreen();
    std::cout << "\033[93mOpening source code repository in 3 seconds...\033[0m\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ShellExecuteA(NULL, "open", "https://github.com/XxSolarXxREAL/boiii-active-connection-fix/tree/main", NULL, NULL, SW_SHOWNORMAL);
    std::cout << "\nYou can explore the source code at the provided link.\n";
    std::cout << "Feel free to review or contribute!\n\n";
    pauseAndReturn();
}

void showCredits() {
    clearScreen();
    std::cout << "\033[95mCredits:\033[0m\n\n";
    std::cout << "This was made by \033[92mxxsolarxx09\033[0m on Discord.\n\n";
    std::cout << "He is actively developing and updating the EZZ client,\n";
    std::cout << "and making fixes for a few known issues.\n\n";
    std::cout << "Right now, the error \"You must have an active internet connection\" is the only thing he has fixed so far.\n";
    std::cout << "But he plans to make more fixes!\n\n";
    pauseAndReturn();
}

const std::string BOIII_FOLDER_NAME = "\\boiii";

void updateBoiiiExe() {
    clearScreen();
    std::cout << "\033[91mWork in progress\033[0m\n";
    pauseAndReturn();
}

void updateFixExe() {
    clearScreen();
    std::cout << "\033[93mDownloading latest \"Boiii active connection fix.exe\"...\033[0m\n";
    std::string localAppData = getLocalAppDataPath();
    if (localAppData.empty()) {
        std::cout << "\033[91mFailed to get local app data path.\033[0m\n";
        pauseAndReturn();
        return;
    }
    std::string targetFolder = localAppData + BOIII_FOLDER_NAME;
    if (!fs::exists(targetFolder)) fs::create_directories(targetFolder);
    std::string fixExePath = targetFolder + "\\Boiii active connection fix.exe";
    std::string url = "https://github.com/XxSolarXxREAL/boiii-active-connection-fix/raw/refs/heads/main/Boiii%20active%20connection%20fix.exe";

    if (!downloadFile(url, fixExePath)) {
        std::cout << "\033[91mDownload failed for \"Boiii active connection fix.exe\".\033[0m\n";
    }
    else {
        std::cout << "\033[92m\"Boiii active connection fix.exe\" updated successfully.\033[0m\n";
        std::cout << "Found in %localappdata%\\boiii\\ \n";
    }
    pauseAndReturn();
}

void updateDataZip() {
    clearScreen();
    std::cout << "\033[93mDownloading latest data.zip...\033[0m\n";
    std::string localAppData = getLocalAppDataPath();
    if (localAppData.empty()) {
        std::cout << "\033[91mFailed to get local app data path.\033[0m\n";
        pauseAndReturn();
        return;
    }
    std::string targetFolder = localAppData + BOIII_FOLDER_NAME;
    if (!fs::exists(targetFolder)) fs::create_directories(targetFolder);
    std::string zipPath = targetFolder + "\\data.zip";
    std::string url = "https://github.com/XxSolarXxREAL/boiii-active-connection-fix/raw/refs/heads/main/data.zip";

    if (!downloadFile(url, zipPath)) {
        std::cout << "\033[91mDownload failed for data.zip.\033[0m\n";
        pauseAndReturn();
        return;
    }
    else {
        std::cout << "\033[92mdata.zip downloaded successfully.\033[0m\n";
    }
    std::cout << "\033[93mExtracting data.zip...\033[0m\n";
    if (!extractZip(zipPath, targetFolder)) {
        std::cout << "\033[91mExtraction failed.\033[0m\n";
    }
    else {
        std::cout << "\033[92mExtraction completed!\033[0m\n";
    }
    pauseAndReturn();
}

void removeDataFolder() {
    clearScreen();
    std::string localAppData = getLocalAppDataPath();
    if (localAppData.empty()) {
        std::cout << "\033[91mFailed to get local app data path.\033[0m\n";
        pauseAndReturn();
        return;
    }
    std::string dataFolderPath = localAppData + BOIII_FOLDER_NAME + "\\data";
    try {
        if (fs::exists(dataFolderPath)) {
            fs::remove_all(dataFolderPath);
            std::cout << "\033[92mData folder removed successfully.\033[0m\n";
        }
        else {
            std::cout << "\033[93mData folder does not exist.\033[0m\n";
        }
    }
    catch (...) {
        std::cout << "\033[91mFailed to remove data folder.\033[0m\n";
    }
    pauseAndReturn();
}

void updateAll() {
    updateBoiiiExe();
    updateFixExe();
    updateDataZip();
}

void supportDiscord() {
    clearScreen();
    std::cout << "\033[93mGo to our EZZ support channels:\n";
    std::cout << "Channel: #bo3-support\n";
    std::cout << "Link: https://discord.com/channels/968255249506791524/968256194508652564\n\n";
    std::cout << "Then type \xA3replace-bo3 and follow the guide.\n\n";
    pauseAndReturn();
}

void updateBlackOps3Exe() {
    clearScreen();
    std::string folderPath;
    std::cout << "\033[93mPlease enter your BlackOps3 game folder path: \033[0m";
    std::getline(std::cin, folderPath);

    if (folderPath.empty()) {
        std::cout << "\033[91mInvalid path.\033[0m\n";
        pauseAndReturn();
        return;
    }

    if (!fs::exists(folderPath)) {
        std::cout << "\033[91mThe specified folder does not exist.\033[0m\n";
        pauseAndReturn();
        return;
    }

    std::string url = "https://cdn.discordapp.com/attachments/968256194508652564/1294401998078218251/BlackOps3.exe?ex=68cfbc8d&is=68ce6b0d&hm=ae77c8600c0507109c8395d8b6502368433e0a22ff1a90dab106ec403c915109&";
    std::string exePath = folderPath + "\\BlackOps3.exe";

    std::cout << "\n\033[93mDownloading the latest BlackOps3.exe...\033[0m\n";

    if (downloadFile(url, exePath)) {
        std::cout << "\033[92mBlackOps3.exe has been updated successfully!\n";
    }
    else {
        std::cout << "\033[91mFailed to download or place the file.\n";
    }
    pauseAndReturn();
}

void mainMenu() {
    while (true) {
        clearScreen();
        drawHeader();
        std::cout << "\033[92m============== Main Menu ==============\033[0m\n\n";
        std::cout << "\033[93m1)\033[0m  View Guide & Instructions\n";
        std::cout << "\033[93m2)\033[0m  Automatic Fix\n";
        std::cout << "\033[93m3)\033[0m  Help & Join Discord\n";
        std::cout << "\033[93m4)\033[0m  Source Code\n";
        std::cout << "\033[93m5)\033[0m  Credits\n";
        std::cout << "\033[93m6)\033[0m  Update boiii-Exe\n";
        std::cout << "\033[93m7)\033[0m  Update \"Boiii active connection fix.exe\"\n";
        std::cout << "\033[93m8)\033[0m  Update data.zip\n";
        std::cout << "\033[93m9)\033[0m  Remove data folder\n";
        std::cout << "\033[93m10)\033[0m  Update All\n";
        std::cout << "\033[93m11)\033[0m  Support - Bad binary loaded into memory\n";
        std::cout << "\033[93m12)\033[0m  Update BlackOps3.exe (Outdated error fix)\n";
        std::cout << "\033[93m13)\033[0m  Exit\n\n";
        std::cout << "\033[96mSelect an option (1-13): \033[0m";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string dummy;
            std::getline(std::cin, dummy);
            std::cout << "\033[91mInvalid input. Please enter a number.\033[0m\n";
            pauseAndReturn();
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            clearScreen();
            std::thread(openGuideURL).detach();
            std::cout << "\n\n\033[92mIn the folder \"boiii\", look for \"Boiii\" or this path:\033[0m\n";
            std::cout << "%localappdata%\\boiii\\\n\n";
            pauseAndReturn();
        } break;
        case 2: {
            clearScreen();
            std::cout << "\033[93mStarting automatic fix...\033[0m\n\n";
            std::string localAppData = getLocalAppDataPath();
            if (localAppData.empty()) {
                std::cout << "\033[91mFailed to get local app data.\033[0m\n";
                pauseAndReturn(); break;
            }
            std::string targetFolder = localAppData + BOIII_FOLDER_NAME;
            if (!fs::exists(targetFolder)) fs::create_directories(targetFolder);
            std::string zipPath = targetFolder + "\\data.zip";
            std::cout << "\033[93mDownloading data...\033[0m\n\n";
            if (!downloadFile("https://github.com/XxSolarXxREAL/boiii-active-connection-fix/raw/refs/heads/main/data.zip", zipPath)) {
                std::cout << "\033[91mDownload failed.\033[0m\n";
                pauseAndReturn(); break;
            }
            std::cout << "\033[93mExtracting...\033[0m\n\n";
            if (!extractZip(zipPath, targetFolder)) {
                std::cout << "\033[91mExtraction failed.\033[0m\n";
                pauseAndReturn(); break;
            }
            std::cout << "\033[92mFix completed! You can now run boiii.exe.\033[0m\n\n";
            pauseAndReturn();
        } break;
        case 3: {
            clearScreen();
            std::cout << "\033[93mWould you like to open the guide website? (Y/N): \033[0m";
            char answer = _getch();
            if (answer == 'Y' || answer == 'y') {
                ShellExecuteA(NULL, "open", "https://forum.ezz.lol/topic/10/bo3-error-active-connection", NULL, NULL, SW_SHOWNORMAL);
            }
            pauseAndReturn();
        } break;
        case 4: showSourceCode(); break;
        case 5: showCredits(); break;
        case 6: updateBoiiiExe(); break;
        case 7: updateFixExe(); break;
        case 8: updateDataZip(); break;
        case 9: removeDataFolder(); break;
        case 10: updateAll(); break;
        case 11: supportDiscord(); break;
        case 12: updateBlackOps3Exe(); break;
        case 13:
            std::cout << "\033[97mExiting...\033[0m\n";
            return;
        default:
            std::cout << "\033[91mInvalid choice. Try again.\033[0m\n";
            pauseAndReturn();
            break;
        }
    }
}

int main() {
    enableAnsiColors();
    mainMenu();
    return 0;
}
