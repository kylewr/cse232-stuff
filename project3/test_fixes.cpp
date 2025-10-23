#include "bank_account.hpp"
#include <iostream>
#include <iomanip>

int main() {
    // Test overdraft
    std::cout << "=== Test Overdraft ===" << std::endl;
    double bal = 100.0;
    overdraft(bal, 50.0);
    std::cout << "After overdraft(100, 50): " << bal << " (expected: 15.00)" << std::endl;
    
    // Test interest_earned doesn't modify balance
    std::cout << "\n=== Test interest_earned ===" << std::endl;
    double balance = 100.00;
    std::string early = "2015-05-03";
    std::string late = "2015-06-30";
    double result = interest_earned(balance, 12, early, late);
    std::cout << "Balance before: 100.00, after: " << balance << " (should stay 100.00)" << std::endl;
    std::cout << "Interest earned: " << result << " (expected: 1.00)" << std::endl;
    
    // Test process_command
    std::cout << "\n=== Test process_command ===" << std::endl;
    std::string line = "2015-09-03 Deposit $100";
    std::string prev_date = "2015-09-02";
    double bal2 = 10.00;
    std::string output = process_command(line, prev_date, bal2, 12.0);
    std::cout << "Output:\n" << output;
    std::cout << "Balance: " << bal2 << " (expected: 110.00)" << std::endl;
    std::cout << "Date: " << prev_date << " (expected: 2015-09-03)" << std::endl;
    
    return 0;
}
