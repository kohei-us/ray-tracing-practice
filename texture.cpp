
#include "texture.h"

solid_color::solid_color() {}
solid_color::solid_color(const color& c) : color_value(c) {}
solid_color::solid_color(double red, double green, double blue) :
    color_value(red, green, blue) {}

color solid_color::value(double u, double v, const point3& p) const
{
    return color_value;
}

