
#include "camera.h"
#include "ray.h"

camera::camera()
{
    const auto aspect_ratio = 16.0 / 9.0;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    origin = point3(0, 0, 0);
    horizontal = vec3(viewport_width, 0, 0); // horizontal vector
    vertical = vec3(0, viewport_height, 0); // vertical vector

    // 3D position of the lower-left corner of the viewport.
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
}

ray camera::get_ray(double u, double v) const
{
    return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
}
