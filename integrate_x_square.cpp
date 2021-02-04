#include "rtweekend.h"

#include <iostream>
#include <iomanip>

using namespace std;

inline double pdf(double x)
{
    return 3.0*x*x/8.0;
}

int main(int argc, char **argv)
{
    int N = 1;
    double sum = 0.0;
    for (int i = 0; i < N; ++i)
    {
        double x = 8*pow(random_double(), 1./3.);
        sum += x * x / pdf(x);
    }

    cout << fixed << setprecision(12);
    cout << "I = " << sum / N << endl;

    return EXIT_SUCCESS;
}
