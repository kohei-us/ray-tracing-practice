
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
}

image_texture::image_texture() :
    width(0), height(0), bytes_per_scanline(0) {}

image_texture::image_texture(const char* filepath)
{
    int components_per_pixel = bytes_per_pixel;

    unsigned char* _data = stbi_load(
        filepath, &width, &height, &components_per_pixel, components_per_pixel);

    if (!_data) {
        std::cerr << "ERROR: Could not load texture image file '" << filepath << "'.\n";
        width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;

    std::vector<unsigned char> buf(_data, _data + bytes_per_scanline * height);
    data.swap(buf);

    stbi_image_free(_data);
}

color image_texture::value(double u, double v, const point3& p) const
{
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (data.empty()) return color(0, 1, 1);

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

    int i = static_cast<int>(u * width);
    int j = static_cast<int>(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width)  i = width - 1;
    if (j >= height) j = height - 1;

    const auto color_scale = 1.0 / 255.0;
    int offset = j * bytes_per_scanline + i * bytes_per_pixel;
    unsigned char r = data.at(offset);
    unsigned char g = data.at(offset+1);
    unsigned char b = data.at(offset+2);

    return color(color_scale * r, color_scale * g, color_scale * b);
}
