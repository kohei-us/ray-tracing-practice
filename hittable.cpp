#include "aabb.h"
#include "hittable.h"

translate::translate(const std::shared_ptr<hittable>& _ptr, const vec3& _offset) :
    ptr(_ptr), offset(_offset)
{}

bool translate::bounding_box(double time0, double time1, aabb& output_box) const
{
    if (!ptr->bounding_box(time0, time1, output_box)) return false;

    output_box = aabb(
        output_box.min() + offset,
        output_box.max() + offset);

    return true;
}

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
