#pragma once
#include <string>
#include <map>
#include "OrderBookEntry.h"
class Wallet
{
public:
    Wallet();
    /** insert currency to the wallet */
    void insertCurrency(std::string type, double quantity);
    /** remove currency from the wallet */
    bool removeCurrency(std::string type, double quantity);
    /** check if the wallet contains this much currency or more */
    bool containsCurrency(std::string type, double quantity);
    /** generate a string representation of the wallet */
    std::string toString();
    /** Checks if the wallet contains sufficient funds for the transaction */
    bool canFulfillOrder(OrderBookEntry order);
    /** Processes the transaction in the wallet */
    void processSale(OrderBookEntry &sale);

    friend std::ostream &operator<<(std::ostream &os, Wallet &wallet);

private:
    std::map<std::string, double> currencies;
};