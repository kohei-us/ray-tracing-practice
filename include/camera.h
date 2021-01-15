#pragma once

#include "vec3.h"

class ray;

/**
 * This class stores the camera position (origin) and viewport information
 * which is rectangular and is located at the focal length away from the
 * origin.
 */
class camera
{
public:
    camera(
        point3 lookfrom, point3 lookat, vec3 vup,
        double vfov, double aspect_ratio, double aperture, double focus_dist,
        double _time0 = 0, double _time1 = 0);

    /**
     * Get a ray at specified pixel position.
     *
     * @param s horizontal position
     * @param t vertical position
     */
    ray get_ray(double s, double t) const;

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    double time0; // shutter open time
    double time1; // shutter close time
};
