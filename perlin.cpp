
#include "perlin.h"
#include "rtweekend.h"

perlin::perlin()
{
    for (int i = 0; i < point_count; ++i)
        ranfloat[i] = random_double();

    perm_x = generate_perm();
    perm_y = generate_perm();
    perm_z = generate_perm();
}

perlin::~perlin() {}

double perlin::noise(const point3& p) const
{
    int i = static_cast<int>(4 * p.x()) & 255;
    int j = static_cast<int>(4 * p.y()) & 255;
    int k = static_cast<int>(4 * p.z()) & 255;

    return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
}

std::array<int, perlin::point_count> perlin::generate_perm()
{
    perm_type ret;
    for (int i = 0; i < point_count; ++i)
        ret[i] = i;

    permute(ret, point_count);
    return ret;
}

void perlin::permute(perm_type& perm, int n)
{
    for (int i = n - 1; i > 0; --i)
    {
        int target = random_int(0, i);
        std::swap(perm[i], perm[target]);
    }
}

