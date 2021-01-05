
#include "camera.h"
#include "ray.h"

camera::camera(
    point3 lookfrom, point3 lookat, vec3 vup,
    double vfov, double aspect_ratio, double aperture, double focus_dist)
{
    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2.0);

    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u; // horizontal vector
    vertical = focus_dist * viewport_height * v; // vertical vector

    // 3D position of the lower-left corner of the viewport.
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

    lens_radius = aperture / 2.0;
}

ray camera::get_ray(double s, double t) const
{
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();
    return ray(
        origin + offset,
        lower_left_corner + s * horizontal + t * vertical - origin - offset);
}
