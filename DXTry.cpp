// DXTry.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

#include "ConfigClass.h"
#include "SystemClass.h"

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";

    ConfigClass cc(argc, argv);

    SystemClass sc(cc);
    if (sc.Initialize())
        sc.Run();
    sc.Shutdown();
}
