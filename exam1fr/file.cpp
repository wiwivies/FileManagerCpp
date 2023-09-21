#include "file.h"

namespace MyProject
{
    namespace ManagerGeneral
    {
        namespace FileManager
        {
            void File::showContents(const std::string& path) const
            {
                std::ifstream file(path); // ��������� ����
                if (file.is_open()) // ��������� �������� �������� �����
                {
                    std::string line;
                    while (getline(file, line)) // ������ ������ ������ �����
                    {
                        std::cout << line << std::endl;
                    }
                    file.close();
                }
                else
                {
                    throw std::exception(("Error opening file: " + path).c_str());
                }
            }

            void File::createItem(const std::string& name)
            {
                std::ofstream file(name);
                if (file.is_open())
                {
                    std::cout << "File created successfully: " << name << std::endl;
                    file.close();
                }
                else
                {
                    throw std::exception(("Error creating file: " + name).c_str());
                }
            }

            void File::removeItem(const std::string& name)
            {
                if (remove(name.c_str()) == 0)
                {
                    std::cout << "File removed successfully: " << name << std::endl;
                }
                else
                {
                    throw std::exception(("Error removing file: " + name).c_str());
                }
            }

            void File::renameItem(const std::string& oldName, const std::string& newName)
            {
                if (rename(oldName.c_str(), newName.c_str()) == 0)
                {
                    std::cout << "File renamed successfully: " << newName << std::endl;
                }
                else
                {
                    throw std::exception(("Error renaming file: " + oldName).c_str());
                }
            }

            void File::copyItem(const std::string& source, const std::string& destination)
            {
                std::ifstream srcFile(source, std::ios::binary); // ��������� �������� ���� � �������� ������
                if (!srcFile.is_open())
                {
                    // ���������� ����������, ���� �������� ���� �� ������� �������
                    throw std::exception(("Error opening source file: " + source).c_str());
                }

                std::ofstream destFile(destination, std::ios::binary); // ������� ������� ���� � �������� ������
                if (!destFile.is_open())
                {
                    throw std::exception(("Error creating destination file: " + destination).c_str());
                }
                // �������� ���������� ��������� ����� � ������� ����
                destFile << srcFile.rdbuf();

                srcFile.close();
                destFile.close();
                std::cout << "File copied successfully from " << source << " to " << destination << std::endl;
            }

            void File::moveItem(const std::string& source, const std::string& destination)
            {
                if (rename(source.c_str(), destination.c_str()) != 0)
                {
                    throw std::exception(("Error moving file from " + source + " to " + destination).c_str());
                }
                std::cout << "File moved successfully from " << source << " to " << destination << std::endl;
            }

            size_t File::getItemSize(const std::string& name) const
            {
                std::ifstream file(name, std::ios::binary | std::ios::ate); // ��������� ���� � �������� ������ � ������������� ������� � ����� �����

                if (!file.is_open())
                {
                    throw std::exception(("Error opening file " + name).c_str());
                }
                std::streampos fileSize = file.tellg(); // �������� ������� ������� � �����, ������� �������� �������� �����
                file.close();

                return static_cast<size_t>(fileSize);
            }

            void File::writeToFile(const std::string& filename, const std::string& content)
            {
                std::ifstream inputFile(filename);
                std::ofstream file;
                file.open(filename, std::ios::out | std::ios::app); // ��������� ���� ��� ������ ��� ��������

                if (file.is_open())
                {
                    if (inputFile.peek() == std::ifstream::traits_type::eof()) // ���������, �������� �� ���� ������
                    {
                        file << content;
                        std::cout << "Content successfully written to file: " << filename << std::endl;
                    }
                    else
                    {
                        char choice;
                        std::cout << "The file already exists and is not empty.\n";
                        std::cout << "Do you want to overwrite it (O) or append to it (A)? ";
                        std::cin >> choice;
                        std::cin.ignore();

                        if (tolower(choice) == 'o')
                        {
                            file.close();
                            file.open(filename, std::ios::out | std::ios::trunc); // ��������� ���� ��� ���������� (truncate)
                            file << content;
                            std::cout << "Content overwritten in file: " << filename << std::endl;
                        }
                        else if (tolower(choice) == 'a')
                        {
                            file.seekp(0, std::ios::end); // ������������� ������� ������ � ����� �����
                            file << content;
                            std::cout << "Content successfully appended to file: " << filename << std::endl;
                        }
                    }
                    file.close();
                }
                else
                {
                    std::cout << "Failed to open file: " << filename << std::endl;
                }
            }

            bool File::compareFiles(const std::string& file1, const std::string& file2)
            {
                std::ifstream f1(file1, std::ifstream::binary);
                std::ifstream f2(file2, std::ifstream::binary);

                if (!f1.is_open() || !f2.is_open())
                {
                    // ���� ��� ��� ����� �� ������� �������
                    return false;
                }

                constexpr std::streamsize bufferSize = 8192; // ������ ������ ��� ��������� �������

                std::vector<char> buffer1(bufferSize);
                std::vector<char> buffer2(bufferSize);

                do
                {
                    f1.read(buffer1.data(), bufferSize);
                    f2.read(buffer2.data(), bufferSize);

                    const std::streamsize bytesRead1 = f1.gcount();
                    const std::streamsize bytesRead2 = f2.gcount();

                    if (bytesRead1 != bytesRead2 || memcmp(buffer1.data(), buffer2.data(), bytesRead1) != 0)
                    {
                        // ������� ������� ����� �������
                        return false;
                    }
                } while (f1.good() || f2.good());

                // ���������, ��� ��� ����� �������� ����� ������������
                if (!f1.eof() || !f2.eof())
                {
                    // ������� ������ �� ���������
                    return false;
                }

                // ����� ���������
                return true;
            }
        }
    }
}