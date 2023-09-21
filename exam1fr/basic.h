#pragma once
#include <iostream>

namespace MyProject
{
    namespace ManagerGeneral
    {
        // ������� ����� FileManager, ������������ ����� ��������� ��� ���� ����� ��������� ��������� ���������
        class FileManagerBase
        {
        protected:
            std::string path;
        public:
            FileManagerBase(const std::string& filePath) : path(filePath) { }
            std::string getPath() const; // done
            // 1 ����� ��� ����������� ����������� ��������
            virtual void showContents(const std::string& path) const = 0;
            // 2 ����� ��� �������� ��������
            virtual void createItem(const std::string& name) = 0;
            // 3 ����� ��� �������� ��������
            virtual void removeItem(const std::string& name) = 0;
            // 4 ����� ��� �������������� ��������
            virtual void renameItem(const std::string& oldName, const std::string& newName) = 0;
            // 5 ����� ��� ����������� ��������
            virtual void copyItem(const std::string& source, const std::string& destination) = 0;
            // 6 ����� ��� ����������� ��������
            virtual void moveItem(const std::string& source, const std::string& destination) = 0;
            // 7 ����� ��� ��������� ������� ��������
            virtual size_t getItemSize(const std::string& name) const = 0;
        };

        class FileManagerApp;

        namespace DirectoryManager
        {
            class Directory;
        }

        namespace FileManager
        {
            class File;
        }
    }
}