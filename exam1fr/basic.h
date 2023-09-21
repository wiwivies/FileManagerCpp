#pragma once
#include <iostream>

namespace MyProject
{
    namespace ManagerGeneral
    {
        // Базовый класс FileManager, определяющий общий интерфейс для всех типов элементов файлового менеджера
        class FileManagerBase
        {
        protected:
            std::string path;
        public:
            FileManagerBase(const std::string& filePath) : path(filePath) { }
            std::string getPath() const; // done
            // 1 Метод для отображения содержимого элемента
            virtual void showContents(const std::string& path) const = 0;
            // 2 Метод для создания элемента
            virtual void createItem(const std::string& name) = 0;
            // 3 Метод для удаления элемента
            virtual void removeItem(const std::string& name) = 0;
            // 4 Метод для переименования элемента
            virtual void renameItem(const std::string& oldName, const std::string& newName) = 0;
            // 5 Метод для копирования элемента
            virtual void copyItem(const std::string& source, const std::string& destination) = 0;
            // 6 Метод для перемещения элемента
            virtual void moveItem(const std::string& source, const std::string& destination) = 0;
            // 7 Метод для получения размера элемента
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