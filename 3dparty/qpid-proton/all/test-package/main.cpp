#include <iostream>


auto test_qpid()
{
    std::cout << "Conan package test" << std::endl;
}

int main(int argc, char const *argv[])
{
    test_qpid();
    std::cout << "Conan package test completed successfully" << std::endl;
    return 0;
}
