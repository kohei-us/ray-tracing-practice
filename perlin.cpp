
#include "perlin.h"
#include "rtweekend.h"

perlin::perlin()
{
    for (int i = 0; i < point_count; ++i)
    {
        ranfloat[i] = random_double();
        ranvec[i] = unit_vector(vec3::random(-1,1));
    }

    perm_x = generate_perm();
    perm_y = generate_perm();
    perm_z = generate_perm();
}

perlin::~perlin() {}

double perlin::noise(const point3& p) const
{
    double u = p.x() - std::floor(p.x());
    double v = p.y() - std::floor(p.y());
    double w = p.z() - std::floor(p.z());

    int i = static_cast<int>(std::floor(p.x()));
    int j = static_cast<int>(std::floor(p.y()));
    int k = static_cast<int>(std::floor(p.z()));

    vec3 c[2][2][2];

    for (int di = 0; di < 2; ++di)
        for (int dj = 0; dj < 2; ++dj)
            for (int dk = 0; dk < 2; ++dk)
                c[di][dj][dk] = ranvec[
                    perm_x[(i+di) & 255] ^
                    perm_y[(j+dj) & 255] ^
                    perm_z[(k+dk) & 255]
                ];

    return perlin_interp(c, u, v, w);
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

double perlin::trilinear_interp(double c[2][2][2], double u, double v, double w)
{
    double accum = 0.0;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
                accum +=
                    (i*u + (1-i)*(1-u)) *
                    (j*v + (1-j)*(1-v)) *
                    (k*w + (1-k)*(1-w)) *
                    c[i][j][k];

    return accum;
}

double perlin::perlin_interp(vec3 c[2][2][2], double u, double v, double w)
{
    auto f = [](double x) -> double { return x * x * (3 - 2*x); };
    double uu = f(u);
    double vv = f(v);
    double ww = f(w);

    double accum = 0.0;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
            {
                vec3 weight_v(u-i, v-j, w-k);
                accum += (i * uu + (1 - i) * (1 - uu))
                    * (j * vv + (1 - j) * (1 - vv))
                    * (k * ww + (1 - k) * (1 - ww))
                    * dot(c[i][j][k], weight_v);
            }

            return accum;
}

