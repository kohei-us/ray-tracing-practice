#pragma once

#include "rtweekend.h"
#include "vec3.h"
#include <memory>

struct hit_record;
class ray;
class texture;

class material
{
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const color& a);
    lambertian(std::shared_ptr<texture> a);

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

public:
    std::shared_ptr<texture> albedo;
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

class dielectric : public material
{
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

public:
    double ir; // index of refraction

private:
    static double reflectance(double cosine, double ref_idx);
};
