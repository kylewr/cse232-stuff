#include <iostream>
#include <string>

using std::string;
using std::cout, std::endl;

int main() {
    cout << "Hello, World! meow" << endl;

    string dummy{};

    std::cin >> dummy;

    if (dummy == "goto") {
        goto label;
    }

    cout << "no goto for you" << endl;

    // asm volatile (
    //     "nop\n"
    // );

    label:
    cout << "end of prog lol!!! " << endl;
    
    return 0;
}

