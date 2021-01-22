#pragma once

#include "rtweekend.h"
#include "vec3.h"
#include "perlin.h"

#include <memory>
#include <vector>

class texture
{
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture
{
public:
    solid_color();
    solid_color(const color& c);
    solid_color(double red, double green, double blue);

    virtual color value(double u, double v, const point3& p) const override;

private:
    color color_value;
};

class checker_texture : public texture
{
public:
    checker_texture();
    checker_texture(std::shared_ptr<texture> _odd, std::shared_ptr<texture> _even);
    checker_texture(const color& c1, const color& c2);

    virtual color value(double u, double v, const point3& p) const override;

private:
    std::shared_ptr<texture> odd;
    std::shared_ptr<texture> even;
};

class noise_texture : public texture
{
    perlin noise;
    double scale;
public:
    noise_texture();
    noise_texture(double sc);

    virtual color value(double u, double v, const point3 &p) const override;
};

class image_texture : public texture
{
    static constexpr int bytes_per_pixel = 3;

    std::vector<unsigned char> data;
    int width;
    int height;
    int bytes_per_scanline;

public:
    image_texture();
    image_texture(const char* filepath);

    virtual color value(double u, double v, const point3 &p) const override;
};
