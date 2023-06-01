#include <iostream>


int main()
{
    size_t count = 10;
    char test[count]{};
    std::string string_test = "";

    std::cout << sizeof(size_t) << " sizeof type size_t\n";
    std::cout << "Test set to char array: ";
    std::getline(std::cin, string_test);
    const char* char_str = string_test.c_str();

    size_t i = 0;
    while (char_str[i])
    {
        test[i] = char_str[i];
    }

    std::cout << "Out raw char array: \n";
    for (int i = 0; i < count; ++i)
    {
        std::cout << test[i] << '\n';
    }

    const char* str_test = test;
    std::cout << "Out const char str: \n";
    for (size_t i = 0; i < count; i++)
    {
        std::cout << str_test[i] << '\n';
    }
    
    i = 0;
    while (test[i] != 0)
    {
        std::cout << test[i] << '\n';
        ++i;
    }
    
    std::cout << "====\n";
    i = 0;
    while (str_test[i] != 0)
    {
        std::cout << str_test[i] << '\n';
        ++i;
    }

    return 0;
}