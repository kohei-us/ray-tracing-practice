#pragma once

#include "rtweekend.h"
#include "hittable.h"

class material;
class texture;

class constant_medium : public hittable
{
public:
    constant_medium(const std::shared_ptr<hittable>& b, double density, const std::shared_ptr<texture>& t);
    constant_medium(const std::shared_ptr<hittable>& b, double density, const color& c);

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
    std::shared_ptr<hittable> boundary;
    std::shared_ptr<material> phase_function;
    double neg_inv_density;
};
