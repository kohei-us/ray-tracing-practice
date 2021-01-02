
#include "sphere.h"

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    // (P - C)*(P - C) = r^2 where P = A + t*b
    //
    // t^2 * b^2 + 2tb*(A - C) + (A - C)*(A - C) - r^2 = 0
    //
    // a t^2 + b t + c = 0 where
    //
    // oc = A - C
    // a = b^2
    // b = 2 b * oc
    // c = oc * oc - r^2
    //
    // Solving this for t yields:
    //
    // (-b +- sqrt(b^2 - 4ac)) / 2a

    // If b = 2h, then
    // (-h +- sqrt(h^2 - ac)) / a
    // With this, use half_b = h to simplify the equation.
    //
    // discriminant is essentially the expression inside the sqrt.

    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}
