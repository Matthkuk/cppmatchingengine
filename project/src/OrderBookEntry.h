#pragma once

#include <string>

enum class OrderBookType
{
    bid,
    ask,
    unknown,
    bidsale,
    asksale,
};

class OrderBookEntry
{
private:

public:
    /** Create a new OrderBookEntry with the entry
         * set to the sent values
         */
    OrderBookEntry(
        std::string datetime,
        std::string currencyPair,
        OrderBookType side,
        double price,
        double quantity,
        std::string username = "dataset");

    std::string datetime;
    std::string currencyPair;
    OrderBookType side;
    double price;
    double quantity;
    std::string username;

    double getPrice();
    void getEntry();
    static OrderBookType stringToOrderBookType(const std::string &s);
    static bool compareByTimestamp(OrderBookEntry &e1, OrderBookEntry &e2);
    static bool compareByPriceAsc(OrderBookEntry &e1, OrderBookEntry &e2);
    static bool compareByPriceDesc(OrderBookEntry &e1, OrderBookEntry &e2);
};