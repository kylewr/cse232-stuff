#include "exchange.hpp"

void Exchange::MakeDeposit(std::string username, std::string asset, int amount) {
    UserAccount& user = checkOrCreateUser(username);
    user.Deposit(asset, amount);
}

void Exchange::PrintUserPortfolios(std::ostream& os) {
    os << "User Portfolios (in alphabetical order):\n";
    for (const auto& [username, account] : users) {
        os << username << "'s Portfolio: ";
        for (const auto& [asset, amount] : account.GetPortfolio()) {
            if (amount <= 0) {
                continue;
            }

            os << amount << " " << asset << ", ";
        }
        os << '\n';
    }
}

bool Exchange::MakeWithdrawal(std::string username, std::string asset, int amount) {
    if (!userExists(username)) {
        return false;
    }

    UserAccount& user = checkOrCreateUser(username);
    return user.Withdrawal(asset, amount);
}

bool Exchange::AddOrder(Order order) {
    UserAccount& user = checkOrCreateUser(order.username);
    bool result = user.AddOrder(order);
    if (!result) {
        return false;
    }

    if (order.side == "Buy") {
        buy_orders[order.asset].push_back(order);
    } else if (order.side == "Sell") {
        sell_orders[order.asset].push_back(order);
    }

    attemptFillOrders(order);
    return true;
}

void Exchange::PrintUsersOrders(std::ostream& os) {
    os << "Users Orders (in alphabetical order):\n";
    for (auto& [username, account] : users) {
        account.PrintOrders(os);
    }
}

void Exchange::PrintTradeHistory(std::ostream& os) {
    os << "Trade History (in chronological order):\n";
    for (const auto& trade : trade_history) {
        os << trade.buyer_username << " Bought " << trade.amount << " of " << trade.asset
           << " From " << trade.seller_username << " for " << trade.price << " USD\n";
    }
}

void Exchange::PrintBidAskSpread(std::ostream& os) {
    os << "Asset Bid Ask Spread (in alphabetical order):\n";

    std::set<std::string> assets;
    for (const auto& [asset, orders] : buy_orders) {
        if (!orders.empty()) {
            assets.insert(asset);
        }
    }
    for (const auto& [asset, orders] : sell_orders) {
        if (!orders.empty()) {
            assets.insert(asset);
        }
    }

    for (const std::string& asset : assets) {
        int best_buy =
            -1; // wow i love this tech store haha. i wonder if these projects are actually read
        int best_sell = -1;

        if (buy_orders.count(asset)) {
            for (const Order& order : buy_orders[asset]) {
                if (order.amount <= 0) {
                    continue;
                }
                if (best_buy == -1 || order.price > best_buy) {
                    best_buy = order.price;
                }
            }
        }

        if (sell_orders.count(asset)) {
            for (const Order& order : sell_orders[asset]) {
                if (order.amount <= 0) {
                    continue;
                }
                if (best_sell == -1 || order.price < best_sell) {
                    best_sell = order.price;
                }
            }
        }

        os << asset << ": Highest Open Buy = ";
        if (best_buy == -1) {
            os << "NA";
        } else {
            os << best_buy;
        }
        os << " USD and Lowest Open Sell = ";
        if (best_sell == -1) {
            os << "NA";
        } else {
            os << best_sell;
        }
        os << " USD\n";
    }
}

bool Exchange::userExists(const std::string& username) {
    return users.find(username) != users.end();
}

UserAccount& Exchange::checkOrCreateUser(const std::string& username) {
    if (!userExists(username)) {
        users.emplace(username, UserAccount(username));
    }
    return users.at(username);
}

bool Exchange::compareOrders(const Order& a, const Order& b) {
    return a.username == b.username && a.side == b.side && a.asset == b.asset && a.price == b.price;
}

void Exchange::attemptFillOrders(const Order& taker) {
    std::vector<Order>& buyBook = buy_orders[taker.asset];
    std::vector<Order>& sellBook = sell_orders[taker.asset];

    std::vector<Order>* book;
    std::vector<Order>* oppositeBook;

    if (taker.side == "Buy") {
        book = &buyBook;
        oppositeBook = &sellBook;
    } else {
        book = &sellBook;
        oppositeBook = &buyBook;
    }

    int indexOfOrder = -1;
    for (int i = static_cast<int>(book->size()) - 1; i >= 0; --i) {
        Order& buyOrSellOrder = book->at(i);
        if (compareOrders(buyOrSellOrder, taker)) {
            indexOfOrder = i;
            break;
        }
    }

    if (indexOfOrder != -1) {
        Order& orderReference = book->at(indexOfOrder);
        while (orderReference.amount > 0) {
            int bestIndex = -1;
            int bestPrice = -1;

            for (int i = 0; i < static_cast<int>(oppositeBook->size()); ++i) {
                Order& orderItem = oppositeBook->at(i);

                if (taker.side == "Buy") {
                    if (orderItem.amount <= 0 || orderItem.price > orderReference.price) {
                        continue;
                    }
                    if (bestIndex == -1 || orderItem.price < bestPrice) {
                        bestIndex = i;
                        bestPrice = orderItem.price;
                    }
                } else if (taker.side == "Sell") {
                    if (orderItem.amount <= 0 || orderItem.price < orderReference.price) {
                        continue;
                    }
                    if (orderItem.price > bestPrice) {
                        bestPrice = orderItem.price;
                        bestIndex = i;
                    }
                }
            }
            if (bestIndex == -1) {
                break;
            }

            Order& oppositeReference = oppositeBook->at(bestIndex);
            int trade_amount = std::min(orderReference.amount, oppositeReference.amount);
            int trade_price = orderReference.price;
            orderReference.amount -= trade_amount;
            oppositeReference.amount -= trade_amount;

            Trade trade;
            if (taker.side == "Buy") {

                trade = {orderReference.username, oppositeReference.username, orderReference.asset, trade_amount,
                    trade_price};
            } else {
                trade = {oppositeReference.username, orderReference.username, orderReference.asset, trade_amount, trade_price};
            }
            trade_history.push_back(trade);
            UserAccount& buyerAcct = checkOrCreateUser(trade.buyer_username);
            UserAccount& sellerAcct = checkOrCreateUser(trade.seller_username);

            Order& buyerOrderRef = (taker.side == "Buy") ? orderReference : oppositeReference;
            Order& sellerOrderRef = (taker.side == "Buy") ? oppositeReference : orderReference;

            buyerAcct.PerformBuy(buyerOrderRef, trade);
            sellerAcct.PerformSell(sellerOrderRef, trade);
        }
    }

    removeFilledOrders(taker.asset, buyBook, sellBook);
}

void Exchange::removeFilledOrders(const std::string& asset, std::vector<Order>& buys,
                                  std::vector<Order>& sells) {
    std::vector<Order> remaining;
    remaining.reserve(buys.size());
    for (const auto& b : buys)
        if (b.amount > 0) {
            remaining.push_back(b);
        }
    buys.swap(remaining);

    remaining.clear();
    remaining.reserve(sells.size());
    for (const auto& s : sells)
        if (s.amount > 0) {
            remaining.push_back(s);
        }
    sells.swap(remaining);
}
