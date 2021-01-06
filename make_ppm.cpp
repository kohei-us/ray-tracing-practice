#include "color.h"
#include "ray.h"
#include "rtweekend.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

#include <iostream>

using namespace std;

color ray_color(const ray& r, const hittable& world, int depth)
{
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0, 0, 0);
    }

    // Draw background color.
    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0); // scale t to [0-1]
    color c1(1.0, 1.0, 1.0);
    color c2(0.5, 0.7, 1.0);
    return (1.0 - t) * c1 + t * c2;
}

hittable_list random_scene()
{
    hittable_list world;

    auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            double choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_mat < 0.8)
                {
                    // diffuse
                    color albedo = color::random() * color::random();
                    world.add(
                        std::make_shared<sphere>(
                            center, 0.2,
                            std::make_shared<lambertian>(albedo)));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    color albedo = color::random(0.5, 1.0);
                    double fuzz = random_double(0, 0.5);
                    world.add(
                        std::make_shared<sphere>(
                            center, 0.2,
                            std::make_shared<metal>(albedo, fuzz)));
                }
                else
                {
                    // glass
                    world.add(
                        std::make_shared<sphere>(
                            center, 0.2,
                            std::make_shared<dielectric>(1.5)));
                }
            }
        }
    }

    {
        auto mat = std::make_shared<dielectric>(1.5);
        world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, mat));
    }

    {
        auto mat = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
        world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, mat));
    }

    {
        auto mat = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
        world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, mat));
    }

    return world;
}

int main(int argc, char** argv)
{
    // Image shape
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 20;
    const int max_depth = 50;

    // World
    hittable_list world = random_scene();

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    double dist_to_focus = 10.0;
    double aperture = 0.1;
    double field_of_view = 20.0;

    camera cam(
        lookfrom, lookat, vup,
        field_of_view,
        aspect_ratio,
        aperture, dist_to_focus
    );

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                double u = (i + random_double()) / (image_width - 1);
                double v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";

    return EXIT_SUCCESS;
}
