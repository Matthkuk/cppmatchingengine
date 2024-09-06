#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{
}
std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::ifstream csvFile{csvFilename};
    if (!csvFile.is_open())
    {
        throw std::runtime_error("Problem opening file " + csvFilename);
    }

    std::cout << "Opened file " << csvFilename << std::endl;
    std::string line;
    int count = 0;
    int eCount = 0;
    int mCount = 0;
    std::vector<OrderBookEntry> orderbook;
    while (std::getline(csvFile, line))
    {
        try
        {
            std::vector<std::string> tokens = tokenise(line, ',');
            OrderBookEntry obe = stringsToOBE(tokens);
            orderbook.push_back(obe);
            count++;
        }
        catch (const std::runtime_error &e)
        {
            if (e.what() == std::string("Missing data | Bad line"))
            {
                mCount++;
            }
            else
            {
                std::cout << "Error: " << e.what() << std::endl;
                eCount++;
            }
            continue;
        }
        catch (const std::exception &e)
        {
            std::cout << "Unexpected exception: " << e.what() << std::endl;
            eCount++;
            continue;
        }
    }
    std::cout << "Read lines: " << count << std::endl;
    std::cout << "Errors: " << eCount << std::endl;
    std::cout << "Missing information: " << mCount << std::endl;
    std::cout << "Lines in orderbook: " << orderbook.size() << std::endl;
    csvFile.close();
    return orderbook;
}
std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do
    {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end)
            break;
        if (end >= 0)
            token = csvLine.substr(start, end - start);
        else
            token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end != std::string::npos);
    return tokens;
}
OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    std::string timestamp, tradingPair;
    OrderBookType side;
    double price, quantity;
    if (tokens.size() != 5)
    {
        throw std::runtime_error("Missing data | Bad line");
    }
    try
    {
        timestamp = tokens[0];
        tradingPair = tokens[1];
        side = OrderBookEntry::stringToOrderBookType(tokens[2]);
        price = std::stod(tokens[3]);
        quantity = std::stod(tokens[4]);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Exception: ") + e.what());
    }
    OrderBookEntry obe{timestamp, tradingPair, side, price, quantity};
    return obe;
}

OrderBookEntry CSVReader::stringsToOBE(std::string datetime,
                                   std::string currencyPair,
                                   OrderBookType side,
                                   std::string priceStr,
                                   std::string quantityStr,
                                   std::string username)
{
    double price, quantity;
    try
    {
        price = std::stod(priceStr);
        quantity = std::stod(quantityStr);
    }
    catch (const std::exception &e)
    {
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceStr << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << quantityStr << std::endl;
        throw; // throw up to the calling function
    }
    OrderBookEntry obe{datetime,
                       currencyPair,
                       side,
                       price,
                       quantity,
                       username};
    return obe;
}