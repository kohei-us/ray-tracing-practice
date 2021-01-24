#pragma once

#include "rtweekend.h"
#include "hittable.h"

/**
 * 2D rectangle in X-Y plane where Z is constant (=k).
 */
class xy_rect : public hittable
{
public:
    xy_rect();
    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, const std::shared_ptr<material>& _mp);

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
    std::shared_ptr<material> mp;
    double x0, x1, y0, y1, k;
};
