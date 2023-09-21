#include "Directory.h"

namespace MyProject
{
    namespace ManagerGeneral
    {
        namespace DirectoryManager
        {
            void Directory::searchItemsRecursive(const std::string& currentPath, const std::string& mask, std::vector<std::string>& foundFiles/*matchingItems*/) const
            {
                intptr_t handle;
                _finddata_t fileData;
                // Формируем путь для поиска с использованием маски
                std::string searchPath = currentPath + "/" + mask;
                // Используем функцию _findfirst для начала поиска файлов и каталогов в указанном пути
                handle = _findfirst(searchPath.c_str(), &fileData);
                // Если удалось открыть хэндл, выполняем цикл по найденным элементам
                if (handle != -1)
                {
                    do
                    {
                        // Проверяем, является ли текущий элемент файлом (а не каталогом)
                        if ((fileData.attrib & _A_SUBDIR) == 0)
                        {
                            // Получаем имя файла и добавляем его в список найденных файлов
                            std::string filename(fileData.name);
                            foundFiles.push_back(currentPath + "/" + filename);
                        }
                    } while (_findnext(handle, &fileData) == 0);
                    _findclose(handle);
                }
                handle = _findfirst((currentPath + "/*").c_str(), &fileData);
                if (handle != -1)
                {
                    do
                    {
                        if ((fileData.attrib & _A_SUBDIR) != 0 && strcmp(fileData.name, ".") != 0 && strcmp(fileData.name, "..") != 0)
                        {
                            std::string subdirPath = currentPath + "/" + fileData.name;
                            // Рекурсивно вызываем функцию searchItemsRecursive для поиска вложенных элементов в этом каталоге
                            searchItemsRecursive(subdirPath, mask, foundFiles);
                        }
                    } while (_findnext(handle, &fileData) == 0);

                    _findclose(handle);
                }
            }

            void Directory::showContents(const std::string& path) const
            {
                _finddata_t fileInfo; // для хранения информации о файле или папке
                // Инициализация поиска файлов в указанном каталоге
                intptr_t searchHandle = _findfirst((path + "\\*.*").c_str(), &fileInfo);

                if (searchHandle == -1) // Проверка на успешное открытие каталога
                {
                    throw std::exception(("Error accessing directory: " + path).c_str());
                }
                // Перебор всех файлов и папок в каталоге
                while (_findnext(searchHandle, &fileInfo) != -1)
                {
                    // Исключение текущего и родительского каталогов
                    if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
                    {
                        // Формирование пути для каждого элемента
                        std::string itemPath = path + "\\" + fileInfo.name;
                        if (fileInfo.attrib & _A_SUBDIR) // Если элемент является подкаталогом
                        {
                            std::cout << "[" << fileInfo.name << "]" << std::endl;
                            try
                            {
                                showContents(itemPath); // Рекурсивный вызов для обработки подкаталога
                            }
                            catch (const std::exception& ex)
                            {
                                // Обработка исключения, возникшего в рекурсивном вызове
                                std::cout << "Error in subdirectory: " << ex.what() << std::endl;
                            }
                        }
                        else // Если элемент является файлом
                        {
                            std::cout << fileInfo.name << " - " << fileInfo.size << " bytes" << std::endl;
                        }
                    }
                }
                _findclose(searchHandle);
            }

            void Directory::createItem(const std::string& name)
            {
                std::string itemPath = path + "\\" + name;

                if (_mkdir(itemPath.c_str()) == -1) // Если не удалось создать папку
                {
                    throw std::exception(("Error creating directory: " + itemPath).c_str());
                }
                else
                {
                    std::cout << "Directory created successfully: " << itemPath << std::endl;
                }
            }

            void Directory::removeItem(const std::string& name)
            {
                std::string itemPath = path + "/" + name;

                _finddata_t fileInfo;
                intptr_t handle = _findfirst((itemPath + "/*").c_str(), &fileInfo);

                if (handle != -1)
                {
                    do
                    {
                        std::string fileName = fileInfo.name;
                        std::string filePath = itemPath + "/" + fileName;

                        if (strcmp(fileInfo.name, ".") == 0 || strcmp(fileInfo.name, "..") == 0)
                        {
                            continue;// Пропускаем текущую и родительскую папки
                        }

                        if ((fileInfo.attrib & _A_SUBDIR))
                        {
                            Directory directory(filePath);
                            directory.removeItem("."); // Рекурсивно удаляем внутреннюю папку
                        }
                        else
                        {
                            if (_unlink(filePath.c_str()) == 0)
                            {
                                std::cout << "File removed successfully: " << filePath << std::endl;
                            }
                            else
                            {
                                throw std::exception(("Failed to remove file: " + filePath).c_str());
                            }
                        }
                    } while (_findnext(handle, &fileInfo) == 0); // -1 - все элементы в директории были обработаны

                    _findclose(handle);

                    if (_rmdir(itemPath.c_str()) == 0)
                    {
                        std::cout << "Directory removed successfully: " << itemPath << std::endl;
                    }
                    else
                    {
                        throw std::exception(("Failed to remove directory: " + itemPath).c_str());
                    }
                }
                else
                {
                    throw std::exception(("Failed to find directory: " + itemPath).c_str());
                }
            }

            void Directory::renameItem(const std::string& oldName, const std::string& newName)
            {
                std::string oldPath = path + "\\" + oldName; // Формируем путь к исходной папке
                std::string newPath = path + "\\" + newName;

                if (rename(oldPath.c_str(), newPath.c_str()) != 0) 
                {
                    throw std::exception(("Error renaming directory: " + oldPath).c_str());
                }
                else
                {
                    std::cout << "Directory renamed successfully: " << newPath << std::endl;
                }
            }

            void Directory::copyItem(const std::string& source, const std::string& destination)
            {
                int result = _mkdir(destination.c_str()); // Создаем новую папку в назначении

                if (result == 0)
                {
                    std::cout << "Directory created successfully!" << std::endl;

                    _finddata_t fileInfo;
                    intptr_t handle = _findfirst((source + "/*").c_str(), &fileInfo); // Поиск первого файла/папки в исходной папке

                    if (handle != -1)
                    {
                        do
                        {
                            std::string fileName = fileInfo.name;
                            std::string sourceFilePath = source + "/" + fileName;
                            std::string destinationFilePath = destination + "/" + fileName;

                            if (strcmp(fileInfo.name, ".") == 0 || strcmp(fileInfo.name, "..") == 0)
                            {
                                continue;
                            }

                            if ((fileInfo.attrib & _A_SUBDIR))
                            {
                                copyItem(sourceFilePath, destinationFilePath); // Рекурсивно копируем внутренние папки
                            }
                            else
                            {
                                std::ifstream sourceFile(sourceFilePath, std::ios::binary);
                                std::ofstream destinationFile(destinationFilePath, std::ios::binary);

                                if (sourceFile && destinationFile)
                                {
                                    destinationFile << sourceFile.rdbuf(); // Копируем содержимое файла
                                    std::cout << "File copied successfully: " << destinationFilePath << std::endl;
                                }
                                else
                                {
                                    throw std::exception(("Failed to copy file: " + sourceFilePath).c_str());
                                }
                            }
                        } while (_findnext(handle, &fileInfo) == 0); 
                    }
                    _findclose(handle);
                }
                else
                {
                    throw std::exception(("Failed to create directory: " + destination).c_str());
                }
            }

            void Directory::moveItem(const std::string& source, const std::string& destination)
            {
                int result = _mkdir(destination.c_str());

                if (result == 0)
                {
                    std::cout << "Directory created successfully!" << std::endl;

                    _finddata_t fileInfo;
                    intptr_t handle = _findfirst((source + "/*").c_str(), &fileInfo); // Поиск первого файла/папки в исходной папке

                    if (handle != -1)
                    {
                        do
                        {
                            std::string fileName = fileInfo.name;
                            std::string sourceFilePath = source + "/" + fileName;
                            std::string destinationFilePath = destination + "/" + fileName;

                            if (strcmp(fileInfo.name, ".") == 0 || strcmp(fileInfo.name, "..") == 0)
                            {
                                continue;
                            }

                            if ((fileInfo.attrib & _A_SUBDIR))
                            {
                                moveItem(sourceFilePath, destinationFilePath); // Рекурсивно перемещаем внутренние папки
                            }
                            else
                            {
                                std::ifstream sourceFile(sourceFilePath, std::ios::binary);
                                std::ofstream destinationFile(destinationFilePath, std::ios::binary);

                                if (sourceFile && destinationFile)
                                {
                                    destinationFile << sourceFile.rdbuf(); // Копируем содержимое файла
                                    std::cout << "File moved successfully: " << destinationFilePath << std::endl;
                                    _unlink(sourceFilePath.c_str()); // Удаляем исходный файл
                                }
                                else
                                {
                                    throw std::exception(("Failed to move file: " + sourceFilePath).c_str());
                                }
                            }
                        } while (_findnext(handle, &fileInfo) == 0); // Переходим к следующему файлу/папке
                    }
                    _findclose(handle);
                    removeItem(source); // Удаляем исходную папку
                    std::cout << "Directory moved successfully!" << std::endl;
                }
                else
                {
                    throw std::exception(("Failed to create directory: " + destination).c_str());
                }
            }

            size_t Directory::getItemSize(const std::string& name) const
            {
                std::string itemPath = path + "\\" + name;
                size_t totalSize = 0;
                _finddata_t fileInfo;
                intptr_t handle = _findfirst((itemPath + "/*").c_str(), &fileInfo);

                if (handle != -1)
                {
                    do
                    {
                        std::string fileName = fileInfo.name;
                        std::string filePath = itemPath + "/" + fileName;

                        if (strcmp(fileInfo.name, ".") == 0 || strcmp(fileInfo.name, "..") == 0)
                        {
                            continue;
                        }

                        if ((fileInfo.attrib & _A_SUBDIR))
                        {
                            Directory directory(filePath);
                            totalSize += directory.getItemSize("."); // Рекурсивно добавляем размер внутренней папки
                        }
                        else
                        {
                            totalSize += fileInfo.size; // Добавляем размер файла к общему размеру папки
                        }
                    } while (_findnext(handle, &fileInfo) == 0);

                    _findclose(handle);
                }
                return totalSize;
            }

            // *.*  *.txt   name.*  
            std::vector<std::string> Directory::searchItems(const std::string& mask) const
            {
                std::vector<std::string> foundFiles;
                searchItemsRecursive(path, mask, foundFiles);
                return foundFiles;
            }
        }
    }
}