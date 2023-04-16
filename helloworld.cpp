module; // <- Must always be declared as, so it is recognized as a module

/// Includes
#include <iostream>

// Modul declaration
export module helloworld;

export import :german;
export import :english;

/// Imports

/// Code which will not exported

void speak()
{
    std::cout << "Universal: Hello World Universal!" << std::endl;
}

/// Code which will be exported
export void hello()
{
    speak();

    gruss();
    //greet();
}

