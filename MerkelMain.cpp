#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "TimeConverter.h"
#include "LinearRegressor.h"
#include <chrono>
#include <sys/time.h>
#include <ctime>

using namespace std;

MerkelMain::MerkelMain()
{
}

void MerkelMain::init()
{
    std::string input;
    currentTime = orderBook.getEarliestTime();
    
    time_t t1 = time(nullptr);
    cout << "time started: " << t1 << endl;

    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu()
{
    std::cout << "advisorbot> Please enter a command, or help for a list of commands " << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "advisorbot> The available commands are help, help <cmd>, avg, prod, min, max, predict, time, step, stepBack, match " << std::endl;
}

void MerkelMain::printHelpCmd(std::string cmd)
{
    std::cout << "advisorbot> ";
    if(cmd=="avg"){
        std::cout << "Avg ETH/BTC bid 10 -> average ETH/BTC bid over last 10 time steps" << std::endl;
    }
    else if(cmd=="prod"){
        std::cout << "List available products. eg: prod -> ETH/BTC,DOGE/BTC" << std::endl;
    }
    else if(cmd=="min"){
        std::cout << "Find minimum bid or ask for product in current time step, eg: min ETH/BTC ask -> The min ask for ETH/BTC is 1.0" << std::endl;
    }
    else if(cmd=="max"){
        std::cout << "Find maximum bid or ask for product in current time step, eg: max ETH/BTC ask -> The max ask for ETH/BTC is 1.0" << std::endl;
    }
    else if(cmd=="predict"){
        std::cout << "predict max or min ask or bid for the sent product for the next time step" << std::endl;
    }
    else if(cmd=="time"){
        std::cout << "state current time in dataset, i.e. which timeframe are we looking at" << std::endl;
    }
    else if(cmd=="step"){
        std::cout << "move to next time step" << std::endl;
    }
    else if(cmd=="stepBack"){
        std::cout << "move to time step before current one(ie: go back in time)" << std::endl;
    }
    else if(cmd=="match"){
        std::cout << "Run the matching algorithm for the given product at the current time, eg: match ETC/BTC" << std::endl;
    }
    else{
        std::cout << "The entered command is invalid: " << cmd << std::endl;
    }
}

void MerkelMain::printPrediction(string product, string orderTypeStr, string min_or_max){
     // use linear regresstion with x points timestamps and Y the ask/bid price at that tim for the given product
     // since we may have more than one point for every timestamp I will use the max or min of all orders at a given timestamp to come up with a single x,y point for every timestamp
    string timestamp = currentTime;
    string last_timestamp;
    OrderBookType orderType = OrderBookEntry::stringToOrderBookType(orderTypeStr);
    int count_prices = 0;
    std::vector<unsigned long int> Xs;
    std::vector<double> Ys;

    while (true) {
        std::vector<OrderBookEntry> orders = orderBook.getOrders(orderType, product, timestamp);
        TimeConverter tc {timestamp};
        double price;
        if(min_or_max == "min"){
                price = orderBook.getLowPrice(orders);
        }
        else{
                price = orderBook.getHighPrice(orders);
        }
        Xs.push_back(tc.toSeconds());
        Ys.push_back(price);
        last_timestamp = timestamp;
        cout << "timestamp: "<< timestamp<< endl;
        timestamp = orderBook.getBackTime(timestamp);
        if(timestamp == "" or timestamp == last_timestamp){
            break;
        }
    }
    LinearRegressor lr{Xs, Ys};
    string nextTimestamp = orderBook.getNextTime(currentTime);
    TimeConverter tc {nextTimestamp};
    double prediction = lr.predict(tc.toSeconds());
    cout << "advisorbot> Prediction for the " << orderTypeStr << " price of "<<product << " is: "<< prediction << endl;
}   

void MerkelMain::printAvg(std::string product, std::string orderTypeStr, std::string no){

    double sum =0;
    OrderBookType orderType = OrderBookEntry::stringToOrderBookType(orderTypeStr);

    std::string timestamp = orderBook.getLatestTime();

    int no_iterations = stoi(no);
    int count_prices = 0;
    for (int i = 0; i < no_iterations; i++) {
        std::vector<OrderBookEntry> orders = orderBook.getOrders(orderType, product, timestamp);
        timestamp = orderBook.getBackTime(timestamp);
        for (OrderBookEntry& e : orders)
        {
            sum += e.price;
            count_prices +=1;
        }
    }


    double avg;
    if(count_prices >0){
        avg = sum/count_prices;
    }
    else{
        avg = 0;
    }
    std::cout << "advisorbot> The average "<< product << " " << orderTypeStr << " price over the last "<< no << " timesteps was "<< avg << std::endl;
}

void MerkelMain::printProds(){
        cout << "advisorbot> Products are: ";
        for (string p : orderBook.getKnownProducts()){
                  cout << p <<", ";
        }
        cout << endl;
}

void MerkelMain::gotoNextTimeframe()
{
    currentTime = orderBook.getNextTime(currentTime);
    cout << "Moved to " << currentTime << endl;
}

void MerkelMain::gotoPastTimeframe()
{
    currentTime = orderBook.getBackTime(currentTime);
    cout << "Moved to " << currentTime << endl;
}

std::string MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "user> " << std::endl;

    std::getline(std::cin, line);
    return line;
}

void MerkelMain::printMin(std::string product, std::string orderTypeStr)
{
    OrderBookType orderType = OrderBookEntry::stringToOrderBookType(orderTypeStr);
    std::vector<OrderBookEntry> orders = orderBook.getOrders(orderType, 
                                                    product, currentTime);
    double min = orderBook.getLowPrice(orders);
    cout << "The min "<< orderTypeStr <<" for "<< product <<" is " << min << endl;
}

void MerkelMain::printMax(std::string product, std::string orderTypeStr)
{
    OrderBookType orderType = OrderBookEntry::stringToOrderBookType(orderTypeStr);
    std::vector<OrderBookEntry> orders = orderBook.getOrders(orderType, 
                                                    product, currentTime);
    double max = orderBook.getHighPrice(orders);
    cout << "The max "<< orderTypeStr <<" for "<< product <<" is " << max << endl;
}

void MerkelMain::printTime()
{
    cout << "Now at "<< currentTime << endl;
}

void MerkelMain::processUserOption(std::string userInput)
{
    std::vector<std::string> tokens = CSVReader::tokenise(userInput, ' ');
    std::string command = tokens[0];
    if(command == "help")
    {
        if(tokens.size()==1){
            printHelp();
        }
        else{
            std::string cmd = tokens[1];
            printHelpCmd(cmd);
        }
    }
    else if(command == "avg"){
        std::string product = tokens[1];
        std::string orderType = tokens[2];
        std::string no = tokens[3];
        printAvg(product, orderType, no);
    }
    else if(command=="prod"){
        printProds();
    }
    else if(command=="min"){
        std::string product = tokens[1];
        std::string orderTypeStr = tokens[2];
        printMin(product, orderTypeStr);
    }
    else if(command=="max"){
        std::string product = tokens[1];
        std::string orderTypeStr = tokens[2];
        printMax(product, orderTypeStr);
    }
    else if(command=="time"){
        printTime();
    }
    else if(command=="step"){
        gotoNextTimeframe();
    }
    else if(command=="stepBack"){
        gotoPastTimeframe();
    }
    else if(command=="predict"){
        string min_or_max = tokens[1];
        string product = tokens[2];
        string orderTypeStr = tokens[3];
        printPrediction(product, orderTypeStr, min_or_max);
    }
    else if(command=="match"){
        string product = tokens[1];
        time_t t1 = time(nullptr);
        cout << "t1: " << t1 << endl;
        orderBook.matchAsksToBids(product, currentTime);
        time_t t2 = time(nullptr);
        cout << "t2 " << t2 << endl;
        cout << "milliseconds taken by matchAsksToBids: " << t2-t1 << endl;
    }
    else{
        std::cout << "Invalid command: "<< command <<std::endl;
    }
}
