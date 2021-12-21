#include <vector>
using namespace std;

class LinearRegressor{

    public:
        LinearRegressor(vector<unsigned long int> Xs_, vector<double> Ys_);
        double predict(unsigned long int x);

    private:
        void fit(vector<unsigned long int> Xs_, vector<double> Ys_);

        double a, b;


};