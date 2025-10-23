#include "bank_account.hpp"
#include <iostream>

int main() {
    std::string input = 
        "2016-09-02 Deposit $500\n"
        "2016-09-02 Withdraw $23.54\n"
        "2016-09-04 Withdraw $58\n";
    
    std::string result = process_commands(input, 7);
    std::cout << "Result:\n" << result << std::endl;
    
    return 0;
}
