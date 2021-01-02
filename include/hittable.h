#pragma once

#include "ray.h"

struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        // If the ray's direction is opposite of the surface normal, the ray
        // is hitting the front of the surface.
        front_face = dot(r.direction(), outward_normal) < 0;

        // Ensure that the normal is always in the opposite direction of the ray.
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};