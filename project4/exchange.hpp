#pragma once
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "useraccount.hpp"
#include "utility.hpp"

class Exchange {
    public:
        void MakeDeposit(std::string username, std::string asset, int amount);

        void PrintUserPortfolios(std::ostream& os);

        bool MakeWithdrawal(std::string username, std::string asset, int amount);

        bool AddOrder(Order order);

        void PrintUsersOrders(std::ostream& os);

        void PrintTradeHistory(std::ostream& os);

        void PrintBidAskSpread(std::ostream& os);

    private:
        bool userExists(const std::string& username);

        UserAccount& checkOrCreateUser(const std::string& username);

        bool compareOrders(const Order& a, const Order& b);

        void attemptFillOrders(const Order& taker);
        void removeFilledOrders(const std::string& asset, std::vector<Order>& buys,
                                std::vector<Order>& sells);

        std::map<std::string, UserAccount> users;
        std::vector<Trade> trade_history;

        std::map<std::string, std::vector<Order>> buy_orders;
        std::map<std::string, std::vector<Order>> sell_orders;
};
