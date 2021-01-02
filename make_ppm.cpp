#include "color.h"
#include "ray.h"
#include "rtweekend.h"
#include "sphere.h"
#include "hittable_list.h"

#include <iostream>

using namespace std;

color ray_color(const ray& r, const hittable& world)
{
    hit_record rec;
    if (world.hit(r, 0, infinity, rec))
        // Hit an object. Use surface normal as color;
        return 0.5 * (rec.normal + color(1, 1, 1));

    // Draw background color.
    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0); // scale t to [0-1]
    return (1.0 - t) * color(1.0, 0.5, 0.2) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char** argv)
{
    // Image shape
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

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
            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

    return EXIT_SUCCESS;
}
