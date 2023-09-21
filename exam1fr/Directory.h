#pragma once
#include <iostream>
#include <fstream>
#include <io.h>
#include <direct.h>
#include "basic.h"
#include "vector"

namespace MyProject
{
    namespace ManagerGeneral
    {
        namespace DirectoryManager
        {
            // Класс Directory, представляющий директорию
            class Directory : public FileManagerBase
            {
            protected:
                void searchItemsRecursive(const std::string& currentPath, const std::string& mask, std::vector<std::string>& matchingItems) const;// done
            public:
                Directory(const std::string& directoryPath) : FileManagerBase(directoryPath) {}

                void showContents(const std::string& path) const override; // done
                void createItem(const std::string& name) override; // done
                void removeItem(const std::string& name) override; // done
                void renameItem(const std::string& oldName, const std::string& newName) override; // done
                void copyItem(const std::string& source, const std::string& destination) override; // done
                void moveItem(const std::string& source, const std::string& destination) override; // done

                size_t getItemSize(const std::string& name) const override; // done
                // 8 Добавлен метод для поиска элементов по маске
                std::vector<std::string> searchItems(const std::string& mask) const; // done
            };
        }
    }
}