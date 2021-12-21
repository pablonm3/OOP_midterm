#include "LinearRegressor.h"
#include <iostream>
#include <vector>
using namespace std;

LinearRegressor::LinearRegressor(vector<unsigned long int> Xs_, vector<double> Ys_){

        fit(Xs_, Ys_);

}

void LinearRegressor::fit(vector<unsigned long int> Xs, vector<double> Ys){
    float sumX=0, sumX2=0, sumY=0, sumXY=0;
    int n = Xs.size();
    for(int i=0;i<n;i++)
        {
        sumX = sumX + Xs[i];
        sumX2 = sumX2 + Xs[i]*Xs[i];
        sumY = sumY + Ys[i];
        sumXY = sumXY + Xs[i]*Ys[i];
        }
    /* Calculating a and b */
    b = (n*sumXY-sumX*sumY)/(n*sumX2-sumX*sumX);
    a = (sumY - b*sumX)/n;
}

double LinearRegressor::predict(unsigned long int x){
    return a + b*x;
}

