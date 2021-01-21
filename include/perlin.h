#pragma once

#include "vec3.h"
#include <array>

class perlin
{
public:
    perlin();
    ~perlin();

    double noise(const point3& p) const;

private:
    static constexpr int point_count = 256;
    using perm_type = std::array<int, point_count>;

    static perm_type generate_perm();
    static void permute(perm_type& perm, int n);
    static double trilinear_interp(double c[2][2][2], double u, double v, double w);

    std::array<double, point_count> ranfloat;
    perm_type perm_x;
    perm_type perm_y;
    perm_type perm_z;
};
