#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>
class OrderBook
{
public:
    /** construct, reading a csv data file */
    OrderBook(std::string filename);
    /** return vector of all know products in the dataset*/
    std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters*/
    std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                          const std::string &product,
                                          const std::string &timestamp);
    /** return the price of the highest bid in the sent set */
    static double getHighPrice(std::vector<OrderBookEntry> &orders);
    /** return the price of the lowest bid in the sent set */
    static double getLowPrice(std::vector<OrderBookEntry> &orders);
    /** return the spread of the book */
    static double getSpread(std::vector<OrderBookEntry> &orders);

    static double getMidPrice(std::vector<OrderBookEntry> &orders);

    double getSMA(const std::string &timestamp, const std::string &product, int period);
    /** return the earliest time string */
    std::string getEarliestTime();

    std::string getNextTime(const std::string &timestamp);

    void insertOrder(OrderBookEntry &entry);

    static bool sortOrderBook(OrderBookEntry order1, OrderBookEntry order2);

    std::vector<OrderBookEntry> matchAsksToBids(std::string currencyPair, std::string datetime);

private:
    std::vector<OrderBookEntry> orders;
};