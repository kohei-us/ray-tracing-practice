#pragma once

#include "rtweekend.h"
#include "hittable.h"

class moving_sphere : public hittable
{
public:
    moving_sphere();
    moving_sphere(point3 cen0, point3 cen1, double _time0, double _time1, double r, std::shared_ptr<material> m);

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    point3 center(double time) const;

public:
    point3 center0, center1;
    double time0, time1;
    double radius;
    std::shared_ptr<material> mat_ptr;
};
