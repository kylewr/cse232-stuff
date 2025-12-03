#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "utility.hpp"

class UserAccount {
    public:
        UserAccount(std::string name) : username(name) {}

        void Deposit(std::string asset, int amount);

        bool Withdrawal(std::string asset, int amount);

        bool AddOrder(Order order);

        void PerformBuy(Order buy_order, Trade trade);

        void PerformSell(Order sell_order, Trade trade);

        std::map<std::string, int> GetPortfolio() const {
            return portfolio;
        };

        std::vector<Order> GetOpenOrders() const {
            return open_orders;
        };

        void PrintOrders(std::ostream& os);
    
    private:
        Order& findOpenOrder(Order order);
        bool equalOrderID(const Order& a, const Order& b);
        void ensureValidOpenOrders();

        std::string username;
        std::map<std::string, int> portfolio;
        std::vector<Order> open_orders;
        std::vector<Order> filled_orders;

};
