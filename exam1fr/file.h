#pragma once
#include <iostream>
#include <fstream>
#include <io.h>
#include <string>
#include <direct.h>
#include "vector"

#include "basic.h"
namespace MyProject
{
    namespace ManagerGeneral
    {
        namespace FileManager
        {
            class File : public FileManagerBase
            {
            public:
                File(const std::string& filePath) : FileManagerBase(filePath) { } // done

                void showContents(const std::string& path) const override; // done
                void createItem(const std::string& name) override; // done
                void removeItem(const std::string& name) override; // done
                void renameItem(const std::string& oldName, const std::string& newName) override; // done
                void copyItem(const std::string& source, const std::string& destination) override; // done
                void moveItem(const std::string& source, const std::string& destination) override; // done
                size_t getItemSize(const std::string& name) const override; // done
                // 8 вписать что то в файл
                void writeToFile(const std::string& filename, const std::string& content); // done
                // 9 сравнение файлов на подобие
                bool compareFiles(const std::string& file1, const std::string& file2); // done
            };
        }
    }
}