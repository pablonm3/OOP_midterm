#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

using namespace std;

class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
    private: 
        void printMenu();
        void printHelp();
        void printHelpCmd(string cmd);
        void printMarketStats();
        void gotoNextTimeframe();
        std::string getUserOption();
        void processUserOption(std::string input);
        void printAvg(std::string product, std::string orderType, std::string no);
        void printProds();
        void printMin(std::string product, std::string orderTypeStr);
        void printMax(std::string product, std::string orderTypeStr);
        void printPrediction(string product, string orderTypeStr, string min_or_max);
        void printTime();
        void gotoPastTimeframe();

        std::string currentTime;


	    OrderBook orderBook{"20200601.csv"};
        Wallet wallet;

};
