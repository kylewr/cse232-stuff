#pragma once

#include <string>
#include <sstream>
#include <vector>

#define OVERDRAFT_FEE (double)(35.0)

std::vector<std::string> split(const std::string &s, char delimiter);

std::string process_commands(const std::string &commands, int interest_rate);

std::string process_command(std::string command, std::string &last_date, double &balance, double apr);

double round_to_cent(double val);

void deposit(double &balance, double amount);

bool withdraw(double &balance, double amount);

void overdraft(double &balance, double amount);

double interest_for_month(double &balance, double apr);

void string_date_to_ints(std::string str, int &year, int &month, int &day);

int number_of_first_of_months(const std::string& early, const std::string& later);

double interest_earned(double &balance, int apr, std::string& early, std::string& later);
