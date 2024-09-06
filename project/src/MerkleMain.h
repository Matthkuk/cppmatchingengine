#pragma once
#include <vector>
#include <map>
#include <functional>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class MerkleMain
{
public:
    MerkleMain() = default;
    /** Call this to start the sim */
    void init();

private:
    void printMenu();
    int getUserInput();
    void printHelp();
    void printExchangeStats();
    void enterAsk();
    void enterBid();
    void printWallet();
    void gotoNextTimeframe();
    void handleBadInput();
    void processUserOption(int userOption, std::map<int, std::function<void()>> &menu);

    OrderBook orderBook{"../csvorderbookdata.csv"};
    std::map<int, std::function<void()>> menu;
    std::string currentTime;
    Wallet wallet;
};