
#include "camera.h"
#include "ray.h"

camera::camera(
    point3 lookfrom, point3 lookat, vec3 vup,
    double vfov, double aspect_ratio)
{
    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2.0);

    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto w = unit_vector(lookfrom - lookat);
    auto u = unit_vector(cross(vup, w));
    auto v = cross(w, u);

    origin = lookfrom;
    horizontal = viewport_width * u; // horizontal vector
    vertical = viewport_height * v; // vertical vector

    // 3D position of the lower-left corner of the viewport.
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
}

ray camera::get_ray(double s, double t) const
{
    return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
}
