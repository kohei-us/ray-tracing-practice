#include "color.h"
#include "ray.h"
#include "rtweekend.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <future>

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

std::vector<color> run_row(
    const hittable_list& world, const camera& cam, int row, int image_width, int image_height,
    int samples_per_pixel, int max_depth)
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
            pixel_color += ray_color(r, world, max_depth);
        }

        pixel_colors.push_back(pixel_color);
    }

    return pixel_colors;
}

std::string run_row_batch(
    int row_start, int row_end,
    const hittable_list& world, const camera& cam, int image_width, int image_height,
    int samples_per_pixel, int max_depth)
{
    std::ostringstream os;

    for (int row = row_start; row >= row_end; --row)
    {
        std::vector<color> pixel_colors = run_row(
            world, cam, row, image_width, image_height, samples_per_pixel, max_depth);

        for (const auto& c : pixel_colors)
            write_color(os, c, samples_per_pixel);
    }

    return os.str();
}

using interlaced_str_t = std::vector<std::string>;

interlaced_str_t run_rows_interlaced(
    int init_offset, int interval,
    const hittable_list& world, const camera& cam, int image_width, int image_height,
    int samples_per_pixel, int max_depth, bool progress)
{
    interlaced_str_t ret;

    for (int row = image_height - init_offset - 1; row >= 0; row -= interval)
    {
        if (progress)
            std::cerr << "\rscanlines remaining: " << row << "   " << std::flush;

        std::ostringstream os;

        std::vector<color> pixel_colors = run_row(
            world, cam, row, image_width, image_height, samples_per_pixel, max_depth);

        for (const auto& c : pixel_colors)
            write_color(os, c, samples_per_pixel);

        ret.push_back(os.str());
    }

    return ret;
}

void run_interlaced(
    const unsigned int n_threads,
    const hittable_list& world, const camera& cam, int image_width, int image_height,
    int samples_per_pixel, int max_depth)
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
            world, cam, image_width, image_height, samples_per_pixel, max_depth,
            false);
        futures.push_back(std::move(f));
    }

    std::cerr << "thread starting: " << thread_id << std::endl;
    interlaced_str_t last_res = run_rows_interlaced(
        thread_id, n_threads, world, cam, image_width, image_height,
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

void run_split(
    const unsigned int n_threads,
    const hittable_list& world, const camera& cam, int image_width, int image_height,
    int samples_per_pixel, int max_depth)
{
    const unsigned int heights_per_thread = image_height / n_threads;
    std::cerr << "image height per thread: " << heights_per_thread << std::endl;

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    using future_type = std::future<std::string>;
    std::vector<future_type> futures;

    int row = image_height - 1;
    for (unsigned int i = 0; i < (n_threads - 1); ++i)
    {
        int row_end = row - heights_per_thread + 1;
        std::cerr << "rows: " << row << "-" << row_end << " (" << (row - row_end + 1) << ")" << std::endl;

        future_type f = std::async(
            std::launch::async, &run_row_batch, row, row_end, world, cam, image_width, image_height, samples_per_pixel, max_depth);
        futures.push_back(std::move(f));

        row = row_end - 1;
    }

    int row_end = 0;
    std::cerr << "rows: " << row << "-" << row_end << " (" << (row - row_end + 1) << ")" << std::endl;
    std::string last_chunk = run_row_batch(row, row_end, world, cam, image_width, image_height, samples_per_pixel, max_depth);

    for (future_type& f : futures)
    {
        std::string s = f.get();
        std::cout << s;
    }

    std::cout << last_chunk;

    std::cerr << "\nDone.\n";
}

int main(int argc, char** argv)
{
    const unsigned int n_threads = std::thread::hardware_concurrency();

    std::cerr << "number of threads: " << n_threads << std::endl;

    // Image shape
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 50;

    std::cerr << "image size: (" << image_width << ", " << image_height << ")" << std::endl;

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
        aperture, dist_to_focus, 0.0, 1.0
    );

    // Render
//  run_split(n_threads, world, cam, image_width, image_height, samples_per_pixel, max_depth);
    run_interlaced(n_threads, world, cam, image_width, image_height, samples_per_pixel, max_depth);

    return EXIT_SUCCESS;
}
