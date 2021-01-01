#include "color.h"
#include "ray.h"

#include <iostream>

using namespace std;

double hit_sphere(const point3& center, double radius, const ray& r)
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

    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    return discriminant < 0 ? -1.0 : (-half_b - sqrt(discriminant)) / a;
}

color ray_color(const ray& r)
{
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        // inside the sphere. Use surface normal as color.
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1)); // surface normal at t
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    // outside the sphere. Draw background color.
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0); // scale t to [0-1]
    return (1.0 - t) * color(1.0, 0.5, 0.2) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char** argv)
{
    // Image shape
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0); // horizontal vector
    auto vertical = vec3(0, viewport_height, 0); // vertical vector

    // 3D position of the lower-left corner of the viewport.
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    cerr << "origin: " << origin << endl;
    cerr << "horizontal: " << horizontal << endl;
    cerr << "vertical: " << vertical << endl;
    cerr << "lower left corner: " << lower_left_corner << endl;

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            auto u = double(i) / (image_width - 1);  // horizontal increment
            auto v = double(j) / (image_height - 1); // vertical increment
            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

    return EXIT_SUCCESS;
}
