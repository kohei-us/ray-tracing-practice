
#include "texture.h"

solid_color::solid_color() {}
solid_color::solid_color(const color& c) : color_value(c) {}
solid_color::solid_color(double red, double green, double blue) :
    color_value(red, green, blue) {}

color solid_color::value(double u, double v, const point3& p) const
{
    return color_value;
}

checker_texture::checker_texture() {}

checker_texture::checker_texture(std::shared_ptr<texture> _odd, std::shared_ptr<texture> _even) :
    odd(_odd), even(_even) {}

checker_texture::checker_texture(const color& c1, const color& c2) :
    odd(std::make_shared<solid_color>(c1)), even(std::make_shared<solid_color>(c2)) {}

color checker_texture::value(double u, double v, const point3& p) const
{
    auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if (sines < 0)
        return odd->value(u, v, p);
    else
        return even->value(u, v, p);
}

noise_texture::noise_texture() : scale(1.0) {}
noise_texture::noise_texture(double sc) : scale(sc) {}

color noise_texture::value(double u, double v, const point3 &p) const
{
    return color(1, 1, 1) * noise.turb(p * scale);
}
