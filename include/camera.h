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
    camera();

    ray get_ray(double u, double v) const;

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};