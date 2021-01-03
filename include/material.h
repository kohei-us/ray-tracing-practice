#pragma once

#include "rtweekend.h"
#include "vec3.h"

struct hit_record;
class ray;

class material
{
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

public:
    color albedo;
};

class metal : public material
{
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1.0 ? f : 1.0) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

public:
    color albedo;
    double fuzz;
};
