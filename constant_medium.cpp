
#include "constant_medium.h"
#include "material.h"

constant_medium::constant_medium(
    const std::shared_ptr<hittable>& b, double density, const std::shared_ptr<texture>& t) :
    boundary(b), phase_function(std::make_shared<isotropic>(t)), neg_inv_density(-1/density)
{}

constant_medium::constant_medium(
    const std::shared_ptr<hittable>& b, double density, const color& c) :
    boundary(b), phase_function(std::make_shared<isotropic>(c)), neg_inv_density(-1/density)
{}

bool constant_medium::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    hit_record rec1, rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    if (!boundary->hit(r, rec1.t+0.0001, infinity, rec2))
        return false;

    if (rec1.t < t_min)
        rec1.t = t_min;

    if (rec2.t > t_max)
        rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const double ray_length = r.direction().length();
    const double distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const double hit_distance = neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    rec.normal = vec3(1,0,0);  // arbitrary
    rec.front_face = true;     // also arbitrary
    rec.mat_ptr = phase_function;

    return true;
}

bool constant_medium::bounding_box(double time0, double time1, aabb &output_box) const
{
    return boundary->bounding_box(time0, time1, output_box);
}
