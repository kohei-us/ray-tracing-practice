#pragma once

#include "hittable.h"

class sphere : public hittable
{
public:
    sphere() { }
    sphere(point3 cen, double r) :
        center(cen), radius(r) { }

    sphere(point3 cen, double r, const std::shared_ptr<material>& mat) :
        center(cen), radius(r), mat_ptr(mat) { }

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    point3 center;
    double radius;
    std::shared_ptr<material> mat_ptr;
};
