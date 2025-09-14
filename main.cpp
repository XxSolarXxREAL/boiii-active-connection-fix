#include <iostream>
#include <string>
#include <windows.h>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <shlobj.h>
#include <filesystem>
#include <conio.h>

namespace fs = std::filesystem;

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void drawHeader() {
    setColor(11);
    std::cout << R"(
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
                                                                                                      
                                                                                                    
)" << "\n\n";
    setColor(15);
}

void pauseAndReturn() {
    setColor(7);
    std::cout << "\n\nPress any key to return to the main menu...";
    _getch();
}

void clearScreen() {
    system("cls");
}

void showGuide() {
    clearScreen();
    setColor(14);
    std::cout << "==================== HELP & GUIDE ====================\n\n";
    std::cout << "This program helps you fix the BO3 error related to active connection.\n";
    std::cout << "Use the menu options to either view a guide, perform automatic fixes,\n";
    std::cout << "or get help and join our Discord.\n\n";
    std::cout << "--------------------------------------------------------\n";
    std::cout << "1. Guide:\n   Opens a webpage with detailed instructions.\n";
    std::cout << "2. Fix:\n   Downloads and extracts necessary files automatically.\n";
    std::cout << "3. Help:\n   Shows this guide and offers to join Discord.\n";
    std::cout << "4. Exit:\n   Close the program.\n\n";
    pauseAndReturn();
}

void openGuideURL() {
    setColor(14);
    std::cout << "Preparing to open the guide in 10 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(10));
    ShellExecuteA(NULL, "open", "https://forum.ezz.lol/topic/10/bo3-error-active-connection", NULL, NULL, SW_SHOWNORMAL);
}

void openDiscord() {
    setColor(14);
    std::cout << "Redirecting to Discord in 5 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ShellExecuteA(NULL, "open", "https://discord.gg/ezz", NULL, NULL, SW_SHOWNORMAL);
}

std::string getLocalAppDataPath() {
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) {
        return std::string(path);
    }
    return "";
}

bool downloadFile(const std::string& url, const std::string& outputPath) {
    std::string command = "curl -sL -o \"" + outputPath + "\" " + url;
    int result = system(command.c_str());
    return result == 0;
}

bool extractZip(const std::string& zipPath, const std::string& extractTo) {
    std::string command = "powershell -Command \"Expand-Archive -Path '" + zipPath + "' -DestinationPath '" + extractTo + "' -Force\"";
    int result = system(command.c_str());
    return result == 0;
}

void showSourceCode() {
    clearScreen();
    setColor(14);
    std::cout << "Opening source code repository in 3 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ShellExecuteA(NULL, "open", "https://github.com/XxSolarXxREAL/boiii-active-connection-fix/tree/main", NULL, NULL, SW_SHOWNORMAL);

    std::cout << "\nYou can explore the source code at the provided link.\n";
    std::cout << "Feel free to review or contribute!\n\n";
    pauseAndReturn();
}

void showCredits() {
    clearScreen();
    setColor(14);
    std::cout << "Credits:\n\n";
    std::cout << "This was made by xxsolarxx09 on Discord.\n\n";
    std::cout << "He is actively developing and updating the EZZ client,\n";
    std::cout << "and making fixes for a few known issues.\n\n";
    std::cout << "Right now, the error \"You must have an active internet connection\" is the only thing he has fixed so far.\n";
    std::cout << "But he plans to make more fixes!\n\n";
    pauseAndReturn();
}

void mainMenu() {
    while (true) {
        clearScreen();
        drawHeader();
        setColor(15);
        std::cout << "============== Main Menu ==============\n\n";
        std::cout << "1) View Guide & Instructions\n";
        std::cout << "2) Automatic Fix\n";
        std::cout << "3) Help & Join Discord\n";
        std::cout << "4) Source Code\n";
        std::cout << "5) Credits\n";
        std::cout << "6) Exit\n\n";
        std::cout << "Select an option (1-6): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1:
            std::thread(openGuideURL).detach();
            setColor(14);
            std::cout << "\nIn the folder \"boiii\", look for \"Boiii-Remake\" or this path:\n";
            std::cout << "%localappdata%\\Boiii-Remake\\\n";
            pauseAndReturn();
            break;
        case 2:
            clearScreen();
            setColor(14);
            std::cout << "Starting automatic fix...\n";

            {
                std::string localAppData = getLocalAppDataPath();
                if (localAppData.empty()) {
                    setColor(12);
                    std::cout << "Failed to get local app data path.\n";
                    pauseAndReturn();
                    break;
                }
                std::string targetFolder = localAppData + "\\Boiii-Remake";
                std::string zipPath = targetFolder + "\\data.zip";

                try {
                    fs::create_directories(targetFolder);
                }
                catch (...) {
                    setColor(12);
                    std::cout << "Failed to create directory.\n";
                    pauseAndReturn();
                    break;
                }

                setColor(14);
                std::cout << "Downloading data...\n";
                if (!downloadFile("https://github.com/XxSolarXxREAL/boiii-active-connection-fix/raw/refs/heads/main/data.zip", zipPath)) {
                    setColor(12);
                    std::cout << "Download failed.\n";
                    pauseAndReturn();
                    break;
                }

                setColor(14);
                std::cout << "Extracting...\n";
                if (!extractZip(zipPath, targetFolder)) {
                    setColor(12);
                    std::cout << "Extraction failed.\n";
                    pauseAndReturn();
                    break;
                }

                setColor(10);
                std::cout << "Fix completed! You can now run boiii-remake.exe.\n";
                pauseAndReturn();
            }
            break;
        case 3:
            clearScreen();
            showGuide();
            setColor(14);
            std::cout << "\nWould you like to join our Discord server? (Y/N): ";
            (void)_getch();
            char answer;
            answer = _getch();
            if (answer == 'Y' || answer == 'y') {
                openDiscord();
            }
            pauseAndReturn();
            break;
        case 4:
            showSourceCode();
            break;
        case 5:
            showCredits();
            break;
        case 6:
            setColor(15);
            std::cout << "Exiting...\n";
            return;
        default:
            setColor(12);
            std::cout << "Invalid choice. Try again.\n";
            pauseAndReturn();
            break;
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
