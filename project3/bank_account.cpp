#include "bank_account.hpp"
#include <cmath>
#include <iostream>

std::vector<std::string> split(const std::string &s, char delim)
{
    std::istringstream iss(s);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, delim))
    {
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }
    return tokens;
}

std::string process_commands(const std::string &commands, int interest_rate)
{
    std::string output{};
    std::vector<std::string> lines = split(commands, '\n');
    
    std::string last_date;
    double balance = 0.0;

    for (const std::string &line : lines)
    {
        if (line.empty())
            continue;
        
        output += process_command(const_cast<std::string&>(line), last_date, balance, static_cast<double>(interest_rate));
    }

    return output;
}

std::string process_command(std::string command, std::string &last_date, double &balance, double apr)
{
    std::string output{};

    std::vector<std::string> parts = split(command, ' ');
    if (parts.size() < 3)
    {
        return output;
    }

    std::string date = parts.at(0);
    std::string action = parts.at(1);

    std::string amount_str;
    for (size_t i = 2; i < parts.size(); ++i)
    {
        if (i > 2) {
            amount_str += " ";
        }
        amount_str += parts.at(i);
    }

    std::string cleaned;
    for (char c : amount_str)
    {
        if (c == '$' || c == ',')
            continue;
        cleaned.push_back(c);
    }

    double amount = 0.0;
    try
    {
        amount = std::stod(cleaned);
    }
    catch (...) // COPILOT: invalid amount - return empty output
    {
        return output;
    }

    output += "On " + date + ": Instructed to perform \"" + action + " " + amount_str + "\"\n";

    if (!last_date.empty() && last_date != date)
    {
        int numMonths = number_of_first_of_months(last_date, date);
        if (numMonths > 0)
        {
            double interest = interest_earned(balance, static_cast<int>(apr), last_date, date);
            output += "Since " + last_date + ", interest has accrued " + std::to_string(numMonths) + " times.\n";
            std::ostringstream oss;
            oss.setf(std::ios::fixed);
            oss.precision(2);
            oss << "$" << interest << " interest has been earned.\n";
            output += oss.str();
            balance += interest;
        }
    }

    if (action == "Deposit")
    {
        deposit(balance, amount);
    }
    else if (action == "Withdraw")
    {
        if (!withdraw(balance, amount))
        {
            overdraft(balance, amount);
            output += "Overdraft!\n";
        }
    }

    last_date = date;

    // COPILOT: format balance to two decimals
    std::ostringstream bss;
    bss.setf(std::ios::fixed);
    bss.precision(2);
    bss << "Balance: " << round_to_cent(balance) << "\n";
    output += bss.str();

    return output;
}

double round_to_cent(double val)
{
    return std::round(val * 100.0) / 100.0;
}

void deposit(double &balance, double amount)
{
    balance += amount;
}

bool withdraw(double &balance, double amount)
{
    if (balance - amount < 0)
    {
        return false;
    }
    balance -= amount;
    return true;
}

void overdraft(double &balance, double amount)
{
    balance = round_to_cent(balance - amount - OVERDRAFT_FEE);
}

double interest_for_month(double &balance, double apr)
{
    double monthly = balance * (apr / 100.0) / 12.0;
    double floored = std::floor(monthly * 100.0) / 100.0;
    return floored;
}

void string_date_to_ints(std::string str, int &year, int &month, int &day)
{
    std::vector<std::string> parts = split(str, '-');

    year = std::stoi(parts.at(0));
    month = std::stoi(parts.at(1));
    day = std::stoi(parts.at(2));
}

int number_of_first_of_months(const std::string &early, const std::string &later)
{
    int earlyYear, earlyMonth, earlyDay;
    int laterYear, laterMonth, laterDay;
    string_date_to_ints(early, earlyYear, earlyMonth, earlyDay);
    string_date_to_ints(later, laterYear, laterMonth, laterDay);

    int yearDiff = laterYear - earlyYear;
    int monthDiff = laterMonth - earlyMonth;
    int totalMonths = (yearDiff * 12) + monthDiff;

    if (totalMonths < 0)
        totalMonths = 0;

    return totalMonths;
}

double interest_earned(double &balance, int apr, std::string &early, std::string &later)
{
    int numMonths = number_of_first_of_months(early, later);
    if (numMonths <= 0)
    {
        return 0.0;
    }

    double tempBalance = balance;
    double totalInterest = 0.0;
    for (int i = 0; i < numMonths; ++i)
    {
        if (tempBalance > 0)
        {
            double monthly = interest_for_month(tempBalance, apr);
            totalInterest += monthly;
            tempBalance += monthly;
        }
    }

    return round_to_cent(totalInterest);
}

