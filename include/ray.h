#pragma once

#include "vec3.h"

/**
 * This class stores the origin and the direction of a ray in 3D space:
 *
 *   P(t) = A + t dot B
 *
 * where A is the origin and B is the direction of a ray.
 */
class ray
{
public:
    ray() { }
    ray(const point3& origin, const vec3& direction, double time = 0.0)
        : orig(origin), dir(direction), tm(time)
    { }

    point3 origin() const  { return orig; }
    vec3 direction() const { return dir; }
    double time() const { return tm; }

    point3 at(double t) const
    {
        return orig + t * dir;
    }

public:
    point3 orig;
    vec3 dir;
    double tm;
};
