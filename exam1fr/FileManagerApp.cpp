#include "FileManagerApp.h"

namespace MyProject
{
    namespace ManagerGeneral
    {
        void FileManagerApp::run()
        {
            char choice;
            do
            {
                try
                {
                    showMenu();
                    std::cout << "ENTER YOUR CHOICE: ";
                    std::cin >> choice;

                    switch (choice)
                    {
                    case 'a': // work dir
                        currentDirectory.showContents(currentDirectory.getPath());
                        break;
                    case 'b': // work dir
                    {
                        std::string name;
                        std::cout << "Enter directory name: ";
                        std::cin >> name;
                        currentDirectory.createItem(name);
                        break;
                    }
                    case 'c': // work dir
                    {
                        std::string name;
                        std::cout << "Enter directory name: ";
                        std::cin >> name;
                        currentDirectory.removeItem(name);
                        break;
                    }
                    case 'd': // work dir
                    {
                        std::string oldName, newName;
                        std::cout << "Enter old directory name: ";
                        std::cin >> oldName;
                        std::cout << "Enter new directory name: ";
                        std::cin >> newName;
                        currentDirectory.renameItem(oldName, newName);
                        break;
                    }
                    case 'e': // work dir
                    {
                        std::string source, destination;
                        std::cout << "Enter source directory name (with path): ";
                        std::cin >> source;
                        std::cout << "Enter destination directory name (with path): ";
                        std::cin >> destination;
                        currentDirectory.copyItem(source, destination);
                        break;
                    }
                    case 'f': // work dir
                    {
                        std::string source, destination;
                        std::cout << "Enter source directory name: ";
                        std::cin >> source;
                        std::cout << "Enter destination directory name: ";
                        std::cin >> destination;
                        currentDirectory.moveItem(source, destination);
                        break;
                    }
                    case 'g': // work dir
                    {
                        std::string name;
                        std::cout << "Enter item name: ";
                        std::cin >> name;
                        size_t size = currentDirectory.getItemSize(name);
                        std::cout << "Directory size: " << size << " bytes" << std::endl;
                        break;
                    }
                    case 'h':
                    {
                        std::string mask;
                        std::cout << "Enter search mask: ";
                        std::cin >> mask;
                        std::vector<std::string> results = currentDirectory.searchItems(mask);
                        std::cout << "Search results:" << std::endl;
                        for (const std::string& result : results)
                        {
                            std::cout << result << std::endl;
                        }
                        break;
                    }
                    // *    *   *   *   *   *   *   * 
                    case 'i': // work file
                    {
                        std::string name;
                        std::cout << "Enter file name: ";
                        std::cin >> name;
                        currentFile.showContents(name);
                        break;
                    }
                    case 'j': // work file
                    {
                        std::string name;
                        std::cout << "Enter file name: ";
                        std::cin >> name;
                        currentFile.createItem(name);
                        break;
                    }
                    case 'k': // work file
                    {
                        std::string name;
                        std::cout << "Enter file name: ";
                        std::cin >> name;
                        currentFile.removeItem(name);
                        break;
                    }
                    case 'l': // work file
                    {
                        std::string oldName, newName;
                        std::cout << "Enter old file name: ";
                        std::cin >> oldName;
                        std::cout << "Enter new file name: ";
                        std::cin >> newName;
                        currentFile.renameItem(oldName, newName);
                        break;
                    }
                    case 'm': // work file
                    {
                        std::string source, destination;
                        std::cout << "Enter source file name: ";
                        std::cin >> source;
                        std::cout << "Enter destination file name (with path): ";
                        std::cin >> destination;
                        currentFile.copyItem(source, destination);
                        break;
                    }
                    case 'n': // work file
                    {
                        std::string source, destination;
                        std::cout << "Enter source file name: ";
                        std::cin >> source;
                        std::cout << "Enter destination file name (with path): ";
                        std::cin >> destination;
                        currentFile.moveItem(source, destination);
                        break;
                    }
                    case 'o': // work file
                    {
                        std::string name;
                        std::cout << "Enter file name: ";
                        std::cin >> name;
                        size_t size = currentDirectory.getItemSize(name);
                        std::cout << "Item size: " << size << " bytes" << std::endl;
                        break;
                    }
                    case 'p': // work file
                    {
                        std::string filename, content;
                        std::cout << "Enter file name: ";
                        std::cin >> filename;
                        std::cout << "Enter content to write: ";
                        std::cin.ignore(); // Ignore the newline character
                        std::getline(std::cin, content);
                        currentFile.writeToFile(filename, content);
                        break;
                    }
                    case 'r': // work file
                    {
                        std::string file1, file2;
                        std::cout << "Enter first file name: ";
                        std::cin >> file1;
                        std::cout << "Enter second file name: ";
                        std::cin >> file2;
                        bool areEqual = currentFile.compareFiles(file1, file2);
                        if (areEqual)
                        {
                            std::cout << "Files are equal." << std::endl;
                        }
                        else
                        {
                            std::cout << "Files are not equal." << std::endl;
                        }
                        break;
                    }
                    default:
                        break;
                    }
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();  // Игнорировать символы новой строки, оставшиеся во входном буфере
                    std::cin.get();     // Ожидать ввода пользователя перед очисткой экрана и отображением меню
                }
                catch (const std::exception& ex)
                {
                    std::cout << "Exception occurred: " << ex.what() << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                }
            } while (choice != 'q');
        }
    }
}