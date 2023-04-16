module;

#include <iostream>

export module helloworld:german;

export void gruss()
{
    std::cout << "German: Hallo Welt." << std::endl;
}
