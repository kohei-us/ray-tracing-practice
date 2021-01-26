#include "color.h"
#include "ray.h"
#include "rtweekend.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "aarect.h"
#include "box.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <future>

using namespace std;

color ray_color(const ray& r, const color& background, const hittable& world, int depth)
{
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

hittable_list random_scene()
{
    hittable_list world;

    auto checker = std::make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    auto ground_material = std::make_shared<lambertian>(checker);
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
                    auto sphere_material = std::make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(std::make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
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

hittable_list two_spheres()
{
    hittable_list objects;

    auto checker = std::make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(
        std::make_shared<sphere>(
            point3(0, -10, 0),
            10,
            std::make_shared<lambertian>(checker)
        )
    );

    objects.add(
        std::make_shared<sphere>(
            point3(0, 10, 0),
            10,
            std::make_shared<lambertian>(checker)
        )
    );

    return objects;
}

hittable_list two_perlin_spheres()
{
    hittable_list objects;

    auto pertext = std::make_shared<noise_texture>(4.0);

    objects.add(
        std::make_shared<sphere>(
            point3(0, -1000, 0),
            1000,
            std::make_shared<lambertian>(pertext)
        )
    );

    objects.add(
        std::make_shared<sphere>(
            point3(0, 2, 0),
            2,
            std::make_shared<lambertian>(pertext)
        )
    );

    return objects;
}

hittable_list earth()
{
    auto earth_texture = std::make_shared<image_texture>(EARTH_IMAGE_PATH);
    auto earth_surface = std::make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list simple_light()
{
    hittable_list objects;

    auto pertext = std::make_shared<noise_texture>(4);
    objects.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, std::make_shared<lambertian>(pertext)));
    objects.add(std::make_shared<sphere>(point3(0,2,0), 2, std::make_shared<lambertian>(pertext)));

    auto difflight = std::make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

hittable_list cornell_box()
{
    hittable_list objects;

    auto red   = std::make_shared<lambertian>(color(.65, .05, .05));
    auto white = std::make_shared<lambertian>(color(.73, .73, .73));
    auto green = std::make_shared<lambertian>(color(.12, .45, .15));
    auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    objects.add(std::make_shared<box>(point3(130, 0, 65), point3(295, 165, 230), white));
    objects.add(std::make_shared<box>(point3(265, 0, 295), point3(430, 330, 460), white));

    return objects;
}

std::vector<color> run_row(
    const hittable_list& world, const color& background, const camera& cam,
    int row, int image_width, int image_height, int samples_per_pixel, int max_depth)
{
    std::vector<color> pixel_colors;
    for (int i = 0; i < image_width; ++i)
    {
        color pixel_color(0, 0, 0);
        for (int s = 0; s < samples_per_pixel; ++s)
        {
            double u = (i + random_double()) / (image_width - 1);
            double v = (row + random_double()) / (image_height - 1);
            ray r = cam.get_ray(u, v);
            pixel_color += ray_color(r, background, world, max_depth);
        }

        pixel_colors.push_back(pixel_color);
    }

    return pixel_colors;
}

using interlaced_str_t = std::vector<std::string>;

interlaced_str_t run_rows_interlaced(
    int init_offset, int interval,
    const hittable_list& world, const color& background, const camera& cam,
    int image_width, int image_height, int samples_per_pixel, int max_depth,
    bool progress)
{
    interlaced_str_t ret;

    for (int row = image_height - init_offset - 1; row >= 0; row -= interval)
    {
        if (progress)
            std::cerr << "\rscanlines remaining: " << row << "   " << std::flush;

        std::ostringstream os;

        std::vector<color> pixel_colors = run_row(
            world, background, cam, row, image_width, image_height, samples_per_pixel, max_depth);

        for (const auto& c : pixel_colors)
            write_color(os, c, samples_per_pixel);

        ret.push_back(os.str());
    }

    return ret;
}

void run_interlaced(
    const unsigned int n_threads,
    const hittable_list& world, const color& background, const camera& cam,
    int image_width, int image_height, int samples_per_pixel, int max_depth)
{
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    using future_type = std::future<interlaced_str_t>;
    std::vector<future_type> futures;

    unsigned int thread_id = 0;
    for (; thread_id < (n_threads - 1); ++thread_id)
    {
        std::cerr << "thread starting: " << thread_id << std::endl;

        future_type f = std::async(
            std::launch::async, &run_rows_interlaced, thread_id, n_threads,
            world, background, cam, image_width, image_height, samples_per_pixel, max_depth,
            false);
        futures.push_back(std::move(f));
    }

    std::cerr << "thread starting: " << thread_id << std::endl;
    interlaced_str_t last_res = run_rows_interlaced(
        thread_id, n_threads, world, background, cam, image_width, image_height,
        samples_per_pixel, max_depth, true);

    std::vector<interlaced_str_t> results;
    for (future_type& f : futures)
        results.push_back(f.get());

    results.push_back(last_res);

    std::cerr << std::endl;

    for (thread_id = 0; thread_id < n_threads; ++thread_id)
    {
        const interlaced_str_t& res = results[thread_id];
        std::cerr << "thread ending: " << thread_id << "; size: " << res.size() << std::endl;
    }

    // Combine the interlaced string results and write the final output.

    size_t n_last_scan = image_height - results.back().size() * n_threads;

    for (size_t i = 0; i < results.back().size(); ++i)
        for (unsigned int thread_id = 0; thread_id < n_threads; ++thread_id)
            std::cout << results[thread_id][i];

    for (size_t i = 0; i < n_last_scan; ++i)
        std::cout << results[i].back();
}

int main(int argc, char** argv)
{
    int scenario = 1;

    if (argc > 1)
        scenario = std::strtol(argv[1], nullptr, 10);

    const unsigned int n_threads = std::thread::hardware_concurrency();

    std::cerr << "scenario: " << scenario << std::endl;
    std::cerr << "number of threads: " << n_threads << std::endl;

    // Image shape
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 800;
    int samples_per_pixel = 20;
    int max_depth = 50;

    // World

    hittable_list world;
    point3 lookfrom;
    point3 lookat;

    double vfov = 20.0;
    double aperture = 0.0;
    color background(0, 0, 0);

    switch (scenario)
    {
        case 1:
            world = random_scene();
            background = color(0.7, 0.8, 1.0);
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            aperture = 0.1;
            vfov = 20.0;
            break;
        case 2:
            world = two_spheres();
            background = color(0.7, 0.8, 1.0);
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;
        case 3:
            world = two_perlin_spheres();
            background = color(0.7, 0.8, 1.0);
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;
        case 4:
            world = earth();
            background = color(0.7, 0.8, 1.0);
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;
        case 5:
            world = simple_light();
            samples_per_pixel = 400;
            background = color(0, 0, 0);
            lookfrom = point3(26, 3, 6);
            lookat = point3(0, 2, 0);
            vfov = 20.0;
            break;
        case 6:
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            background = color(0, 0, 0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;
        default:
            std::cerr << "invalid scenario index!" << std::endl;
            return EXIT_FAILURE;
    }

    int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    vec3 vup(0,1,0);
    double dist_to_focus = 10.0;

    camera cam(
        lookfrom, lookat, vup,
        vfov,
        aspect_ratio,
        aperture, dist_to_focus, 0.0, 1.0
    );

    std::cerr << "image size: (" << image_width << ", " << image_height << ")" << std::endl;

    // Render
    run_interlaced(n_threads, world, background, cam, image_width, image_height, samples_per_pixel, max_depth);

    return EXIT_SUCCESS;
}
