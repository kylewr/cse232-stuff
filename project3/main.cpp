#include <iostream>
#include <string>

using std::string;
#include "bank_account.hpp"

int main()
{

    double balance {100.0};
    double apr {12};
std::string date_early = "2015-05-03";
std::string date_late = "2015-06-30";

    // std::cout << number_of_first_of_months(date_early, date_late) << std::endl;
    // std::cout << interest_earned(balance, apr, date_early, date_late) << std::endl;
    std::cout << "New balance: " << balance << std::endl;

    // string input{
    //     "2016-09-02 Deposit $500\n"
    //     "2016-09-02 Withdraw $23.54\n"
    //     "2016-09-04 Withdraw $58\n"
    //     "2016-11-02 Deposit $200\n"
    //     "2017-03-02 Withdraw $1000\n"
    //     "2019-11-02 Deposit $500\n"};

    // string result{process_commands(input, 7)};

    // string expected{
    //     "On 2016-09-02: Instructed to perform \"Deposit $500\"\n"
    //     "Balance: 500.00\n"
    //     "On 2016-09-02: Instructed to perform \"Withdraw $23.54\"\n"
    //     "Balance: 476.46\n"
    //     "On 2016-09-04: Instructed to perform \"Withdraw $58\"\n"
    //     "Balance: 418.46\n"
    //     "On 2016-11-02: Instructed to perform \"Deposit $200\"\n"
    //     "Since 2016-09-04, interest has accrued 2 times.\n"
    //     "$4.89 interest has been earned.\n"
    //     "Balance: 623.35\n"
    //     "On 2017-03-02: Instructed to perform \"Withdraw $1000\"\n"
    //     "Since 2016-11-02, interest has accrued 4 times.\n"
    //     "$14.65 interest has been earned.\n"
    //     "Overdraft!\n"
    //     "Balance: -397.00\n"
    //     "On 2019-11-02: Instructed to perform \"Deposit $500\"\n"
    //     "Since 2017-03-02, interest has accrued 32 times.\n"
    //     "$0.00 interest has been earned.\n"
    //     "Balance: 103.00\n"};

    // std::cout << result;
    // if (result != expected)
    // {
    //     std::cout << "ERROR, result doesn't match expected" << std::endl;
    // }
}