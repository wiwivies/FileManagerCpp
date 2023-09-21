#include <iostream>

#include "basic.h"
#include "Directory.h"
#include "FileManagerApp.h"
#include "file.h"

int main() 
{
    std::string path;
    std::cout << "ENTER DIRECTORY PATH: ";
    std::cin >> path;

    MyProject::ManagerGeneral::FileManagerApp app(path);
    app.run();

    return 0;
}