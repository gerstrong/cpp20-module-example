module;

#include <iostream>
#include <string>

export module helloworld;

export void printline(const std::string &msg)
{
    std::cout << msg << std::endl;
}

export void hello()
{
    printline("Hello World!!!");
}
