#include "OrderBookEntry.h"
#include <iostream>

OrderBookEntry::OrderBookEntry(
    std::string datetime,
    std::string currencyPair,
    OrderBookType side,
    double price,
    double quantity,
    std::string username) : datetime(datetime),
                       currencyPair(currencyPair),
                       side(side),
                       price(price),
                       quantity(quantity),
                       username(username) {}
                       
double OrderBookEntry::getPrice()
{
    return price;
}

void OrderBookEntry::getEntry()
{
    std::string side_str = (side == OrderBookType::bid) ? "bid" : "ask";
    std::cout << datetime << "," << currencyPair << "," << side_str << "," << price << "," << quantity << std::endl;
}

OrderBookType OrderBookEntry::stringToOrderBookType(const std::string &s)
{
    if (s == "ask")
    {
        return OrderBookType::ask;
    }
    if (s == "bid")
    {
        return OrderBookType::bid;
    }
    return OrderBookType::unknown;
}

bool OrderBookEntry::compareByTimestamp(OrderBookEntry &e1, OrderBookEntry &e2)
{
    return e1.datetime < e2.datetime;
}

bool OrderBookEntry::compareByPriceAsc(OrderBookEntry &e1, OrderBookEntry &e2)
{
    return e1.price < e2.price;
}

bool OrderBookEntry::compareByPriceDesc(OrderBookEntry &e1, OrderBookEntry &e2)
{
    return e1.price > e2.price;
}