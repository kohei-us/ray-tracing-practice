#pragma once

#include <limits>
#include <random>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

constexpr inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

/** Generate a random double between 0 and 1. */
inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max+1));
}

constexpr inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;

    return x;
}

