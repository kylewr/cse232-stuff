#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "utility.hpp"

class UserAccount {
    public:
        UserAccount(std::string name);

        void Deposit(std::string asset, int amount);

        bool Withdrawal(std::string assert, int amount);

        bool AddOrder(Order order);

        void PerformBuy(Order buy_order, Trade trade);

        void PerformSell(Order sell_order, Trade trade);

        std::map<std::string, int> GetPortfolio();

        std::vector<Order> GetOpenOrders();

        void PrintOrders(std::ostream& os);
};
