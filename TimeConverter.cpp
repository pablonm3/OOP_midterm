#include "TimeConverter.h"
#include <string>
#include "CSVReader.h"
using namespace std;



TimeConverter::TimeConverter(string timestamp){
    // timestamp ex: 2020/06/01 14:56:30.202861
    std::vector<std::string> tokens = CSVReader::tokenise(timestamp, '/');
    year = stoi(tokens[0]);
    month = stoi(tokens[1]);
    tokens = CSVReader::tokenise(tokens[2], ' ');
    day = stoi(tokens[0]);
    tokens = CSVReader::tokenise(tokens[1], ':');
    hour = stoi(tokens[0]);
    minute = stoi(tokens[1]);
    tokens = CSVReader::tokenise(tokens[2], '.');
    second = stoi(tokens[0]);
    ms = stoi(tokens[1]);
}


unsigned long int TimeConverter::toSeconds(){
    int SEC_IN_DAY = 86400;
    unsigned long int timeAsSecs = (2000 - year) * 365 * SEC_IN_DAY;
    timeAsSecs += month * 30 * SEC_IN_DAY;
    timeAsSecs += day * SEC_IN_DAY;
    timeAsSecs += minute * 60;
    timeAsSecs += second;

    return timeAsSecs;
}

