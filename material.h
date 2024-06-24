#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"
class material {
public:
  virtual ~material() = default;

  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       color &attenuation, ray &scattered) const {
    return false;
  };
};

class lambertian : public material {
public:
  lambertian(const color &albedo) : albedo{albedo} {}
  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    auto scattered_direction = random_unit_vector() + rec.normal;

    // edge case where scatter is almost zero
    if (scattered_direction.near_zero()) {
      scattered_direction = rec.normal;
    }

    scattered = ray(rec.p, scattered_direction);
    attenuation = albedo;
    return true;
  };

private:
  color albedo;
};

class metal : public material {
public:
  metal(const color &albedo) : albedo{albedo} {}
  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    auto scattered_direction = reflect(r_in.direction(), rec.normal);
    scattered = ray{rec.p, scattered_direction};
    attenuation = albedo;
    return true;
  };

private:
  color albedo;
};

#endif // !MATERIAL_H
