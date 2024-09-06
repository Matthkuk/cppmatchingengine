#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <utility>

OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV("../csvorderbookdata.csv");
    // std::sort(orders.begin(), orders.end(), sortOrderBook);
}

std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> pairs;
    std::map<std::string, bool> currencyPairs;
    for (const OrderBookEntry &order : orders)
    {
        currencyPairs.insert(std::pair<std::string, bool>{order.currencyPair, true});
    }
    for (auto pair : currencyPairs) {
        pairs.push_back(pair.first);
    }
    return pairs;
}

std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                 const std::string &product,
                                                 const std::string &timestamp)
{
    std::vector<OrderBookEntry> filteredOrders;
    for (const OrderBookEntry &order : orders)
    {
        if ((type == OrderBookType::unknown || order.side == type) &&
            (product.empty() || order.currencyPair == product) &&
            (timestamp.empty() || order.datetime == timestamp))
        {
            filteredOrders.push_back(order);
        }
    }
    return filteredOrders;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders)
{
    double max = orders[0].price;
    for (const OrderBookEntry &order : orders)
    {
        max = std::max(order.price, max);
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders)
{
    double min = orders[0].price;
    for (const OrderBookEntry &order : orders)
    {
        min = std::min(order.price, min);
    }
    return min;
}

double OrderBook::getSpread(std::vector<OrderBookEntry> &orders)
{
    double maxBid = std::numeric_limits<double>::lowest();
    double minAsk = std::numeric_limits<double>::max();
    if (orders.empty())
    {
        return 0.0;
    }
    for (const OrderBookEntry &order : orders)
    {
        if (order.side == OrderBookType::ask) {
            minAsk = std::min(order.price, minAsk);
        }
        else if (order.side == OrderBookType::bid)
        {
            maxBid = std::max(order.price, maxBid);
        }
    }
    if (maxBid == std::numeric_limits<double>::lowest() || minAsk == std::numeric_limits<double>::max())
    {
        return 0.0;
    }
    return minAsk - maxBid;
}

double OrderBook::getMidPrice(std::vector<OrderBookEntry> &orders)
{
    double maxBid = std::numeric_limits<double>::lowest();
    double minAsk = std::numeric_limits<double>::max();
    if (orders.empty())
    {
        return 0.0;
    }
    for (const OrderBookEntry &order : orders)
    {
        if (order.side == OrderBookType::ask)
        {
            minAsk = std::min(order.price, minAsk);
        }
        else if (order.side == OrderBookType::bid)
        {
            maxBid = std::max(order.price, maxBid);
        }
    }
    if (maxBid == std::numeric_limits<double>::lowest())
    {
        return minAsk;
    }
    else if (minAsk == std::numeric_limits<double>::max())
    {
        return maxBid;
    }
    return (minAsk + maxBid) / 2.0;
}

double OrderBook::getSMA(const std::string &timestamp, const std::string &product, int period)
{
    std::string curTimestamp = getEarliestTime();
    std::vector<double> MA;
    while (curTimestamp <= timestamp)
    {
        if (curTimestamp == getEarliestTime())
        {
            MA.clear();
        }
        std::vector<OrderBookEntry> orders = getOrders(OrderBookType::unknown, product, curTimestamp);
        double midPriceOrAverage = getMidPrice(orders);
        if (MA.size() == period)
        {
            MA.erase(MA.begin());
        }
        MA.push_back(midPriceOrAverage);
        curTimestamp = getNextTime(curTimestamp);
    }
    double sum = 0;
    for (double& average : MA)
    {
        sum += average;
    }
    return sum / MA.size();
}

std::string OrderBook::getEarliestTime()
{
    std::string earliest = orders[0].datetime;
    for (auto& order : orders)
    {
        earliest = std::min(earliest, order.datetime);
    }
    return earliest;
}

std::string OrderBook::getNextTime(const std::string &datetime)
{
    std::string next_datetime = "";
    for (OrderBookEntry &e : orders)
    {
        if (e.datetime > datetime)
        {
            next_datetime = e.datetime;
            break;
        }
    }
    if (next_datetime == "")
    {
        next_datetime = getEarliestTime();
    }
    return next_datetime;
}

void OrderBook::insertOrder(OrderBookEntry &entry)
{
    orders.push_back(entry);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

bool OrderBook::sortOrderBook(OrderBookEntry order1, OrderBookEntry order2)
{
    return order1.datetime < order2.datetime;
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string currencyPair, std::string datetime)
{
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, currencyPair, datetime);
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, currencyPair, datetime);

    std::vector<OrderBookEntry> sales;

    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceDesc);

    for (auto& ask : asks) {
        for (auto& bid : bids) {
            if (bid.price >= ask.price) {
                OrderBookEntry sale{datetime,
                                    currencyPair,
                                    OrderBookType::bidsale,
                                    ask.price,
                                    0};
                if (ask.username == "simuser") 
                {
                    sale.side = OrderBookType::asksale;
                    sale.username = "simuser";
                }
                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                }
                if (bid.quantity != 0 && ask.quantity != 0)
                {
                    if (bid.quantity == ask.quantity) {
                        sale.quantity = ask.quantity;
                        sales.push_back(sale);
                        bid.quantity = 0;
                        break;
                    }

                    if (bid.quantity > ask.quantity) {
                        sale.quantity = ask.quantity;
                        sales.push_back(sale);
                        bid.quantity = bid.quantity - ask.quantity;
                        break;
                    }

                    if (bid.quantity < ask.quantity) {
                        sale.quantity = bid.quantity;
                        sales.push_back(sale);
                        ask.quantity = ask.quantity - bid.quantity;
                        bid.quantity = 0;
                        continue;
                    }
                }
            }
        }
    }

    return sales;
}