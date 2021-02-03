#include "rtweekend.h"

#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, char **argv)
{
    int N = 1000000;
    double sum = 0.0;
    for (int i = 0; i < N; ++i)
    {
        double x = random_double(0, 2);
        sum += x * x;
    }

    cout << fixed << setprecision(12);
    cout << "I = " << 2 * sum / N << endl;

    return EXIT_SUCCESS;
}
