#pragma once

#include "rtweekend.h"
#include "vec3.h"

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
