#pragma once

#include "vec3.h"
#include "ray.h"

class aabb
{
public:
    aabb();
    aabb(const point3& a, const point3& b);

    point3 min() const;
    point3 max() const;

    bool hit(const ray& r, double t_min, double t_max) const;

    point3 minimum;
    point3 maximum;
};

aabb surrounding_box(const aabb& box0, const aabb& box1);
