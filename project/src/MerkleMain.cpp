#include "MerkleMain.h"
#include "CSVReader.h"
#include <iostream>

void MerkleMain::printMenu()
{
    std::cout << "1: Print help" << std::endl;
    std::cout << "2: Print exchange stats" << std::endl;
    std::cout << "3: Place an ask" << std::endl;
    std::cout << "4: Place a bid" << std::endl;
    std::cout << "5: Print Wallet" << std::endl;
    std::cout << "6: Continue" << std::endl;
}

int MerkleMain::getUserInput()
{
    int userOption;
    std::cout << "Type in 1-6" << std::endl;
    std::string line;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        throw;
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkleMain::printHelp()
{
    std::cout << "Help- your aim is to make money." << std::endl;
    std::cout << "Analyse the market and make bids" << std::endl;
    std::cout << "and offers. " << std::endl;
}

void MerkleMain::printExchangeStats()
{
    for (const std::string &p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::unknown, p, currentTime);
        std::cout << "Orders seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min bid: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "Spread: " << OrderBook::getSpread(entries) << std::endl;
        std::cout << "Mid Price: " << OrderBook::getMidPrice(entries) << std::endl;
        std::cout << "SMA(5): " << orderBook.getSMA(currentTime, p, 5) << std::endl;
    }
    std::cout << "Current time: " << currentTime << std::endl;
}

void MerkleMain::enterAsk()
{
    std::cout << "What ask would you like to place?" << std::endl;
    std::cout << "Enter the currency pair, price, quantity in that format" << std::endl;
    std::string line;
    std::getline(std::cin, line);
    std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
    try {
        if (tokens.size() != 3)
            throw std::exception();
        OrderBookEntry obe = CSVReader::stringsToOBE(currentTime, tokens[0], OrderBookType::ask, tokens[1], tokens[2], "simuser");
        if (wallet.canFulfillOrder(obe))
        {
            std::cout << "Wallet looks good. " << std::endl;
            orderBook.insertOrder(obe);
        }
        else
        {
            std::cout << "Wallet has insufficient funds . " << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Bad input" << std::endl;
    }
}

void MerkleMain::enterBid()
{
    std::cout << "What bid would you like to place?" << std::endl;
    std::cout << "Enter the currency pair, price, quantity in that format" << std::endl;
    std::string line;
    std::getline(std::cin, line);
    std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
    try
    {
        if (tokens.size() != 3)
            throw std::exception();
        OrderBookEntry obe = CSVReader::stringsToOBE(currentTime, tokens[0], OrderBookType::bid, tokens[1], tokens[2], "simuser");
        if (wallet.canFulfillOrder(obe))
        {
            std::cout << "Wallet looks good. " << std::endl;
            orderBook.insertOrder(obe);
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Bad input" << std::endl;
    }
}

void MerkleMain::printWallet()
{
    std::cout << "Printing wallet..." << std::endl;
    std::cout << wallet;
}

void MerkleMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;

    for (std::string &p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry &sale : sales)
        {
            if (sale.username == "simuser")
            {
                wallet.processSale(sale);
                std::cout << "Sale price: " << sale.price << " amount " << sale.quantity << std::endl;
            }
        }
    }

    currentTime = orderBook.getNextTime(currentTime);
}

void MerkleMain::handleBadInput()
{
    std::cout << "Bad input, try again..." << std::endl;
}

void MerkleMain::processUserOption(int userOption, std::map<int, std::function<void()>> &menu)
{
    if (menu.find(userOption) != menu.end())
    {
        menu[userOption]();
    }
    else
    {
        handleBadInput();
    }
}

void MerkleMain::init()
{
    currentTime = orderBook.getEarliestTime();
    wallet.insertCurrency("BTC", 10);
    menu = {
        {1, std::bind(&MerkleMain::printHelp, this)},
        {2, std::bind(&MerkleMain::printExchangeStats, this)},
        {3, std::bind(&MerkleMain::enterAsk, this)},
        {4, std::bind(&MerkleMain::enterBid, this)},
        {5, std::bind(&MerkleMain::printWallet, this)},
        {6, std::bind(&MerkleMain::gotoNextTimeframe, this)}};
    int input;
    while (true)
    {
        printMenu();
        input = getUserInput();
        processUserOption(input, menu);
    }
}