#include "useraccount.hpp"

void UserAccount::Deposit(std::string asset, int amount) {
    portfolio[asset] += amount;
}

bool UserAccount::Withdrawal(std::string asset, int amount) {
    if (portfolio.find(asset) == portfolio.end()) {
        return false;
    }

    if (portfolio[asset] >= amount) {
        portfolio[asset] -= amount;
        return true;
    }
    return false;
}

bool UserAccount::AddOrder(Order order) {
    if (order.side != "Buy" && order.side != "Sell") {
        return false;
    }

    if (order.side == "Sell") {
        if (portfolio.find(order.asset) == portfolio.end() ||
            portfolio[order.asset] < order.amount) {
            return false;
        }
    } else if (order.side == "Buy") {
        if (portfolio.find("USD") == portfolio.end() ||
            (order.price * order.amount) > portfolio["USD"]) {
            return false;
        }
    }

    open_orders.push_back(order);

    if (order.side == "Sell") {
        portfolio[order.asset] -= order.amount;
    } else if (order.side == "Buy") {
        portfolio["USD"] -= order.price * order.amount;
    }

    return true;
}

void UserAccount::PerformBuy(Order buy_order, Trade trade) {
    portfolio[trade.asset] += trade.amount;

    for (Order& order : open_orders) {
        if (!equalOrderID(order, buy_order)) {
            continue;
        }

        if (order.amount >= trade.amount) {
            order.amount -= trade.amount;
            Order filled_part {buy_order.username, buy_order.side, buy_order.asset, trade.amount,
                               trade.price};
            filled_orders.push_back(filled_part);
            break;
        }
    }

    ensureValidOpenOrders();
}

void UserAccount::PerformSell(Order sell_order, Trade trade) {
    portfolio["USD"] += trade.price * trade.amount;

    for (Order& order : open_orders) {
        if (!equalOrderID(order, sell_order)) {
            continue;
        }

        if (order.amount >= trade.amount) {
            order.amount -= trade.amount;
            Order filled_part {sell_order.username, sell_order.side, sell_order.asset, trade.amount,
                               trade.price};
            filled_orders.push_back(filled_part);
            break;
        }
    }

    ensureValidOpenOrders();
}

void UserAccount::PrintOrders(std::ostream& os) {
    os << username << "'s Open Orders (in chronological order):\n";
    for (const auto& order : open_orders) {
        os << order << '\n';
    }
    os << username << "'s Filled Orders (in chronological order):\n";
    for (const auto& order : filled_orders) {
        os << order << '\n';
    }
}

Order& UserAccount::findOpenOrder(Order order) {
    for (auto& open_order : open_orders) {
        if (open_order == order) {
            return open_order;
        }
    }
    return open_orders[0]; // Should never reach here if used correctly
}

bool UserAccount::equalOrderID(const Order& a, const Order& b) {
    return a.username == b.username && a.side == b.side && a.asset == b.asset && a.price == b.price;
}

void UserAccount::ensureValidOpenOrders() {
    std::vector<Order> remaining;
    for (const Order& order : open_orders) {
        if (order.amount > 0) {
            remaining.push_back(order);
        }
    }
    open_orders.swap(remaining);
}
