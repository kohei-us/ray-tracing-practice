#include "rtweekend.h"

#include <iostream>
#include <iomanip>

using namespace std;

inline double pdf(double x)
{
    return 0.5 * x;
}

int main(int argc, char **argv)
{
    int N = 1000000;
    double sum = 0.0;
    for (int i = 0; i < N; ++i)
    {
        double x = sqrt(random_double(0, 4));
        sum += x * x / pdf(x);
    }

    cout << fixed << setprecision(12);
    cout << "I = " << sum / N << endl;

    return EXIT_SUCCESS;
}
