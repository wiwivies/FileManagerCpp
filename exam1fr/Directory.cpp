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
                // ��������� ���� ��� ������ � �������������� �����
                std::string searchPath = currentPath + "/" + mask;
                // ���������� ������� _findfirst ��� ������ ������ ������ � ��������� � ��������� ����
                handle = _findfirst(searchPath.c_str(), &fileData);
                // ���� ������� ������� �����, ��������� ���� �� ��������� ���������
                if (handle != -1)
                {
                    do
                    {
                        // ���������, �������� �� ������� ������� ������ (� �� ���������)
                        if ((fileData.attrib & _A_SUBDIR) == 0)
                        {
                            // �������� ��� ����� � ��������� ��� � ������ ��������� ������
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
                            // ���������� �������� ������� searchItemsRecursive ��� ������ ��������� ��������� � ���� ��������
                            searchItemsRecursive(subdirPath, mask, foundFiles);
                        }
                    } while (_findnext(handle, &fileData) == 0);

                    _findclose(handle);
                }
            }

            void Directory::showContents(const std::string& path) const
            {
                _finddata_t fileInfo; // ��� �������� ���������� � ����� ��� �����
                // ������������� ������ ������ � ��������� ��������
                intptr_t searchHandle = _findfirst((path + "\\*.*").c_str(), &fileInfo);

                if (searchHandle == -1) // �������� �� �������� �������� ��������
                {
                    throw std::exception(("Error accessing directory: " + path).c_str());
                }
                // ������� ���� ������ � ����� � ��������
                while (_findnext(searchHandle, &fileInfo) != -1)
                {
                    // ���������� �������� � ������������� ���������
                    if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
                    {
                        // ������������ ���� ��� ������� ��������
                        std::string itemPath = path + "\\" + fileInfo.name;
                        if (fileInfo.attrib & _A_SUBDIR) // ���� ������� �������� ������������
                        {
                            std::cout << "[" << fileInfo.name << "]" << std::endl;
                            try
                            {
                                showContents(itemPath); // ����������� ����� ��� ��������� �����������
                            }
                            catch (const std::exception& ex)
                            {
                                // ��������� ����������, ���������� � ����������� ������
                                std::cout << "Error in subdirectory: " << ex.what() << std::endl;
                            }
                        }
                        else // ���� ������� �������� ������
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

                if (_mkdir(itemPath.c_str()) == -1) // ���� �� ������� ������� �����
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
                            continue;// ���������� ������� � ������������ �����
                        }

                        if ((fileInfo.attrib & _A_SUBDIR))
                        {
                            Directory directory(filePath);
                            directory.removeItem("."); // ���������� ������� ���������� �����
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
                    } while (_findnext(handle, &fileInfo) == 0); // -1 - ��� �������� � ���������� ���� ����������

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
                std::string oldPath = path + "\\" + oldName; // ��������� ���� � �������� �����
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
                int result = _mkdir(destination.c_str()); // ������� ����� ����� � ����������

                if (result == 0)
                {
                    std::cout << "Directory created successfully!" << std::endl;

                    _finddata_t fileInfo;
                    intptr_t handle = _findfirst((source + "/*").c_str(), &fileInfo); // ����� ������� �����/����� � �������� �����

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
                                copyItem(sourceFilePath, destinationFilePath); // ���������� �������� ���������� �����
                            }
                            else
                            {
                                std::ifstream sourceFile(sourceFilePath, std::ios::binary);
                                std::ofstream destinationFile(destinationFilePath, std::ios::binary);

                                if (sourceFile && destinationFile)
                                {
                                    destinationFile << sourceFile.rdbuf(); // �������� ���������� �����
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
                    intptr_t handle = _findfirst((source + "/*").c_str(), &fileInfo); // ����� ������� �����/����� � �������� �����

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
                                moveItem(sourceFilePath, destinationFilePath); // ���������� ���������� ���������� �����
                            }
                            else
                            {
                                std::ifstream sourceFile(sourceFilePath, std::ios::binary);
                                std::ofstream destinationFile(destinationFilePath, std::ios::binary);

                                if (sourceFile && destinationFile)
                                {
                                    destinationFile << sourceFile.rdbuf(); // �������� ���������� �����
                                    std::cout << "File moved successfully: " << destinationFilePath << std::endl;
                                    _unlink(sourceFilePath.c_str()); // ������� �������� ����
                                }
                                else
                                {
                                    throw std::exception(("Failed to move file: " + sourceFilePath).c_str());
                                }
                            }
                        } while (_findnext(handle, &fileInfo) == 0); // ��������� � ���������� �����/�����
                    }
                    _findclose(handle);
                    removeItem(source); // ������� �������� �����
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
                            totalSize += directory.getItemSize("."); // ���������� ��������� ������ ���������� �����
                        }
                        else
                        {
                            totalSize += fileInfo.size; // ��������� ������ ����� � ������ ������� �����
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