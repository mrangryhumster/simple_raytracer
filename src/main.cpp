#include "ConsoleApplication.h"
#include "GUIApplication.h"

#include <cstring>

int main(int argc, char** argv)
{
    int result = 0;
    if (argc > 1 && std::strcmp(argv[1], "--console") == 0)
    {
        ConsoleApplication app;
        result = app.Run(argc, argv);
    }
    else
    {
        GUIApplication app;
        result = app.Run(argc, argv);
    }
    //Sleep(10000);
    return result;
}