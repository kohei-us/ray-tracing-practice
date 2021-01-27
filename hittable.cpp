#include "aabb.h"
#include "hittable.h"

translate::translate(const std::shared_ptr<hittable>& _ptr, const vec3& _offset) :
    ptr(_ptr), offset(_offset)
{}

bool translate::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    // Pretend that the hittable object is located at the object's original
    // location. We simply move the ray by the offset amount in reverse
    // direction to see if the ray would hit it...

    ray moved_r(r.origin() - offset, r.direction(), r.time());

    if (!ptr->hit(moved_r, t_min, t_max, rec))
        return false;

    // If the ray hits the object at its original location, we simply move its
    // hit position by the offset.
    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

bool translate::bounding_box(double time0, double time1, aabb& output_box) const
{
    if (!ptr->bounding_box(time0, time1, output_box)) return false;

    output_box = aabb(
        output_box.min() + offset,
        output_box.max() + offset);

    return true;
}

rotate_y::rotate_y(const std::shared_ptr<hittable>& _ptr, double angle) :
    ptr(_ptr)
{
    double radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);

    aabb bbox;
    if (!ptr->bounding_box(0, 1, bbox))
        // the original object has no bounding box!
        return;

    box = std::make_shared<aabb>(bbox); // copy

    // Calculate the bounding box for the rotated object. Note that we need to
    // check 8 vertices in a 3D box.
    point3 min(infinity, infinity, infinity);
    point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                // NB: y positions will remain the same.
                double x = i * bbox.max().x() + (1-i) * bbox.min().x();
                double y = j * bbox.max().y() + (1-j) * bbox.min().y();
                double z = k * bbox.max().z() + (1-k) * bbox.min().z();

                double new_x = cos_theta * x + sin_theta * z;
                double new_z = -sin_theta * x + cos_theta * z;

                vec3 tester(new_x, y, new_z);

                for (int c = 0; c < 3; ++c)
                {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmin(max[c], tester[c]);
                }
            }
        }
    }

    box->minimum = min;
    box->maximum = max;
}

bool rotate_y::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    // Rotate the ray itself by theta but in reverse direction i.e. -theta.
    point3 origin = r.origin();
    vec3 direction = r.direction();

    // NB: cos(-theta) = cos(theta); sin(-theta) = -sin(theta)
    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    point3 p = rec.p;
    vec3 normal = rec.normal;

    // Rotate the hit position and the normal vector back.
    p[0] =  cos_theta * rec.p[0] + sin_theta * rec.p[2];
    p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

    normal[0] =  cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
    normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

bool rotate_y::bounding_box(double time0, double time1, aabb &output_box) const
{
    if (!box)
        return false;

    output_box = *box;
    return true;
}
