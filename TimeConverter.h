#pragma once

#include <string>

using namespace std;

class TimeConverter{

    public:
        TimeConverter(string timestamp);
        unsigned long int toSeconds();


    private:
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
        int ms;

};