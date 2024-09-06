#include "Wallet.h"
#include <utility>
#include <iostream>
#include "CSVReader.h"

Wallet::Wallet()
{

}

/** insert currency to the wallet */
void Wallet::insertCurrency(std::string type, double quantity)
{
    if (quantity <= 0) {
        throw std::exception();
    }

    if (currencies.count(type) == 0)
    {
        currencies.insert(std::pair<std::string, double>(type, quantity));
        std::cout << "Added new currency!" << std::endl;
    }
    else
    {
        currencies[type] += quantity;
        std::cout << "Added to existing currency!" << std::endl;
    }
}

/** remove currency from the wallet */
bool Wallet::removeCurrency(std::string type, double quantity)
{
    if (quantity <= 0)
    {
        throw std::exception();
    }

    if (currencies.count(type) == 0)
    {
        std::cout << "Currency does not exist in Wallet" << std::endl;
        return false;
    }
    else if (containsCurrency(type, quantity))
    {
        currencies[type] -= quantity;
        std::cout << "Removed!" << std::endl;
        return true;
    }
    return false;
}

/** check if the wallet contains this much currency or more */
bool Wallet::containsCurrency(std::string type, double quantity)
{
    if (currencies.count(type) == 0)
    {
        std::cout << "Currency does not exist in Wallet" << std::endl;
        return false;
    }
    else
    {
        if (currencies[type] >= quantity)
        {
            std::cout << "Wallet contains enough currency" << std::endl;
        }
        else
        {
            std::cout << "Wallet does not contain enough currency" << std::endl;
        }
        return currencies[type] >= quantity;
    }
}

/** generate a string representation of the wallet */
std::string Wallet::toString()
{
    std::string str;
    for (std::pair<std::string, double> pair : currencies)
    {
        std::string currency = pair.first;
        double quantity = pair.second;
        str += currency + ": " + std::to_string(quantity) + "\n";
    }
    return str;
}

bool Wallet::canFulfillOrder(OrderBookEntry order)
{
    std::vector<std::string> currencyPair = CSVReader::tokenise(order.currencyPair, '/');
    if (order.side == OrderBookType::bid)
    {
        return containsCurrency(currencyPair[1], order.quantity * order.price);
    }
    else if (order.side == OrderBookType::ask)
    {
        return containsCurrency(currencyPair[0], order.quantity);
    }
    return false;
}

void Wallet::processSale(OrderBookEntry &sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.currencyPair, '/');
    // ask
    if (sale.side == OrderBookType::asksale)
    {
        double outgoingAmount = sale.quantity;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.quantity * sale.price;
        std::string incomingCurrency = currs[1];
        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
    // bid
    if (sale.side == OrderBookType::bidsale)
    {
        double incomingAmount = sale.quantity;
        std::string incomingCurrency = currs[0];
        double outgoingAmount = sale.quantity * sale.price;
        std::string outgoingCurrency = currs[1];
        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}

std::ostream &operator<<(std::ostream &os, Wallet &wallet)
{
    os << wallet.toString();
    return os;
}