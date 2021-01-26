#pragma once

#include "hittable_list.h"

struct hit_record;
class ray;
class aabb;

class box : public hittable
{
public:
    box();
    box(const point3& p0, const point3& p1, const std::shared_ptr<material>& ptr);

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
    point3 box_min;
    point3 box_max;
    hittable_list sides;
};
