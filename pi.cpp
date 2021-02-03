
#include "rtweekend.h"

#include <iostream>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

int main(int argc, char **argv)
{
    int inside_circle = 0;
    int inside_circle_stratified = 0;
    int sqrt_N = 10000;

    for (int i = 0; i < sqrt_N; ++i)
    {
        for (int j = 0; j < sqrt_N; ++j)
        {
            double x = random_double(-1, 1);
            double y = random_double(-1, 1);

            if (x*x + y*y < 1)
                ++inside_circle;

            x = 2 * ((i + random_double()) / sqrt_N) - 1;
            y = 2 * ((j + random_double()) / sqrt_N) - 1;

            if (x*x + y*y < 1)
                ++inside_circle_stratified;
        }
    }

    double N = static_cast<double>(sqrt_N) * sqrt_N;
    cout << fixed << setprecision(12);

    cout << "regular    estimate of pi = " << 4 * double(inside_circle) / N << endl;
    cout << "stratified estimate of pi = " << 4 * double(inside_circle_stratified) / N << endl;
    cout << "                       pi = " << M_PI << endl;

    return EXIT_SUCCESS;
}
