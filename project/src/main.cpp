#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "MerkleMain.h"
#include "Wallet.h"

double computeAveragePrice(std::vector<OrderBookEntry> &entries)
{
    double averagePrice = 0;
    double numberOfEntries = 0;
    for (OrderBookEntry &entry : entries)
    {
        averagePrice += entry.getPrice();
        ++numberOfEntries;
    }
    averagePrice /= numberOfEntries;
    return averagePrice;
}

double computeLowPrice(std::vector<OrderBookEntry> &entries)
{
    double min = std::numeric_limits<double>::max();
    for (OrderBookEntry &entry : entries) {
        min = std::min(min, entry.getPrice());
    }
    return min;
}

double computeHighPrice(std::vector<OrderBookEntry> &entries)
{
    double max = std::numeric_limits<double>::min();
    for (OrderBookEntry &entry : entries)
    {
        max = std::max(max, entry.getPrice());
    }
    return max;
}

double computePriceSpread(std::vector<OrderBookEntry> &entries) {
    return computeHighPrice(entries) - computeLowPrice(entries);
}

int main()
{
    Wallet wallet;

    // // Test insertCurrency function
    // try
    // {
    //     wallet.insertCurrency("BTC", 1.5);
    //     std::cout << "Wallet should contain 1.5 BTC now:" << std::endl;
    //     std::cout << wallet;
    // }
    // catch (const std::exception&)
    // {
    //     std::cout << "Bad Input, enter a positive non-zero number" << std::endl;
    // }

    // try
    // {
    //     bool result = wallet.containsCurrency("BTC", 1.5);
    //     std::cout << "Result should be true: " << result << std::endl;
    // }
    // catch (const std::exception&)
    // {
    //     std::cout << "Exception occurred while checking currency" << std::endl;
    // }

    // // Test removing more currency than available
    // try
    // {
    //     bool result = wallet.removeCurrency("BTC", 2.0);
    //     std::cout << "Result should be false (Not enough currency): " << result << std::endl;
    // }
    // catch (const std::exception&)
    // {
    //     std::cout << "Bad Input, enter a positive non-zero number" << std::endl;
    // }

    // // Test removing an amount that is available
    // try
    // {
    //     bool result = wallet.removeCurrency("BTC", 1.0);
    //     std::cout << "Result should be true (1 BTC removed): " << result << std::endl;
    //     std::cout << "Wallet should now contain 0.5 BTC:" << std::endl;
    //     std::cout << wallet;
    // }
    // catch (const std::exception&)
    // {
    //     std::cout << "Bad Input, enter a positive non-zero number" << std::endl;
    // }

    // // Test containsCurrency function with insufficient funds
    // try
    // {
    //     bool result = wallet.containsCurrency("BTC", 1.0);
    //     std::cout << "Result should be false (Only 0.5 BTC available): " << result << std::endl;
    // }
    // catch (const std::exception&)
    // {
    //     std::cout << "Exception occurred while checking currency" << std::endl;
    // }

    // // Test inserting a new currency
    // try
    // {
    //     wallet.insertCurrency("ETH", 2.0);
    //     std::cout << "Wallet should contain 2 ETH now:" << std::endl;
    //     std::cout << wallet;
    // }
    // catch (const std::exception&)
    // {
    //     std::cout << "Bad Input, enter a positive non-zero number" << std::endl;
    // }

    // // Test inserting a negative quantity
    // try
    // {
    //     wallet.insertCurrency("BTC", -1.0);
    //     std::cout << "Wallet should still contain 0.5 BTC, no changes:" << std::endl;
    //     std::cout << wallet;
    // }
    // catch (const std::exception&)
    // {
    //     std::cout << "Bad Input, enter a positive non-zero number" << std::endl;
    // }

    // // Test removing currency that does not exist
    // try
    // {
    //     bool result = wallet.removeCurrency("USD", 1.0);
    //     std::cout << "Result should be false (USD does not exist in wallet): " << result << std::endl;
    // }
    // catch (const std::exception&)
    // {
    //     std::cout << "Bad Input, enter a positive non-zero number" << std::endl;
    // }

    // // Test removing currency with a negative quantity
    // try
    // {
    //     bool result = wallet.removeCurrency("ETH", -1.0);
    //     std::cout << "Result should be false (Invalid input): " << result << std::endl;
    // }
    // catch (const std::exception&)
    // {
    //     std::cout << "Bad Input, enter a positive non-zero number" << std::endl;
    // }

    // // Test inserting currency again
    // try
    // {
    //     wallet.insertCurrency("BTC", 0.5);
    //     std::cout << "Wallet should contain 1 BTC now:" << std::endl;
    //     std::cout << wallet;
    // }
    // catch (const std::exception&)
    // {
    //     std::cout << "Bad Input, enter a positive non-zero number" << std::endl;
    // }
    MerkleMain app{};
    app.init();
}