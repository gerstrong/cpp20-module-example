module;

#include <iostream>

export module helloworld:english;


export void greet()
{
    std::cout << "English: Hello World." << std::endl;
}
