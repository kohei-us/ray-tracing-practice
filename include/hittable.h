#pragma once

#include "ray.h"

#include <memory>

class material;
class aabb;

struct hit_record
{
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;
    double t;
    double u;
    double v;
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
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

class translate : public hittable
{
public:
    translate(const std::shared_ptr<hittable>& _ptr, const vec3& _offset);

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
    std::shared_ptr<hittable> ptr;
    vec3 offset;
};

class rotate_y : public hittable
{
public:
    rotate_y(const std::shared_ptr<hittable>& _ptr, double angle);

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
    std::shared_ptr<hittable> ptr;
    std::shared_ptr<aabb> box;
    double sin_theta;
    double cos_theta;
};
