#pragma once
#include <iomanip>
#include <iostream>
#include <windows.h>

#include "Directory.h"
#include "file.h"

namespace MyProject
{
    namespace ManagerGeneral
    {
        // Класс FileManagerApp, представляющий приложение файлового менеджера
        class FileManagerApp
        {
        private:
            DirectoryManager::Directory currentDirectory;
            FileManager::File currentFile;
        public:
            FileManagerApp(const std::string& path) : currentDirectory(path), currentFile(path) {}

            void showMenu() const
            {
                system("cls"); // !!!!!
                std::cout << "\033[1;34m"; // Set text color to dark blue

                std::string text = "M E N U:";
                int consoleWidth = 80; 
                size_t textWidth = text.length();

                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
                GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
                int consoleWidthActual = consoleInfo.dwSize.X; // Фактическая ширина консоли

                size_t padding = (consoleWidth - textWidth) / 2; // Вычисляем количество пробелов слева от текста
                int leftMargin = (consoleWidthActual - consoleWidth) / 2; // Вычисляем отступ слева для центрирования

                std::cout << std::setw(leftMargin) << "" << std::setw(padding) << std::right << text << "\n\n";

                std::cout << "\033[1;31m";  // dark red
                std::cout << "DIRECTORY:\n\n";
                std::cout << "a. Show contents of the directory\n"; // done
                std::cout << "b. Create directory\n"; // done
                std::cout << "c. Remove directory\n"; // done
                std::cout << "d. Rename directory\n"; // done
                std::cout << "e. Copy directory\n"; // done
                std::cout << "f. Move directory\n"; // done
                std::cout << "g. Get directory size\n"; // done
                std::cout << "h. Search items by mask\n\n"; // done

                std::cout << "\033[0;33m";  // dark orange
                std::cout << "FILE:\n\n";
                std::cout << "i. Show contents of the file\n"; // done
                std::cout << "j. Create file\n"; // done
                std::cout << "k. Remove file\n"; // done
                std::cout << "l. Rename file\n"; // done
                std::cout << "m. Copy file\n"; // done
                std::cout << "n. Move file\n"; // done
                std::cout << "o. Get file size\n"; // done
                std::cout << "p. Write to file\n"; // done
                std::cout << "r. Compare two files\n\n"; // done

                std::cout << "\033[1;30m";  // gray
                std::cout << "q. Quit\n\n"; // done

                std::cout << "\033[0m";  // Reset text color to default
            }

            void run(); // done
        };

    }
}