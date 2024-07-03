#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"
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

    scattered = ray(rec.p, scattered_direction, r_in.time());
    attenuation = albedo;
    return true;
  };

private:
  color albedo;
};

class metal : public material {
public:
  metal(const color &albedo, double fuzz)
      : albedo{albedo}, fuzz{fuzz < 1 ? fuzz : 1} {}
  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    auto reflected = reflect(r_in.direction(), rec.normal);
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered = ray{rec.p, reflected, r_in.time()};
    attenuation = albedo;
    return true;
  };

private:
  color albedo;
  double fuzz;
};

class dielectric : public material {
public:
  dielectric(double refraction_index) : refraction_index{refraction_index} {}

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {

    attenuation = color(0.95, 0.95, 0.95);
    double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;
    vec3 unit_dir = unit_vector(r_in.direction());

    double cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;
    if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
      direction = reflect(unit_dir, rec.normal);
    } else {
      direction = refract(unit_dir, rec.normal, ri);
    }

    scattered = ray{rec.p, direction, r_in.time()};
    return true;
  }

private:
  double refraction_index;
  static double reflectance(double cosine, double refraction_index) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};

class weird : public material {
public:
  weird(const vec3 &stretch) : stretch{stretch} {}

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    attenuation = {0.9, 0.9, 0.9};
    if (rec.front_face || true) {
      scattered = ray{rec.p, r_in.direction() * stretch};
    } else {
      scattered = ray{rec.p, r_in.direction() * vec3(1.0 / stretch.x(),
                                                     1.0 / stretch.y(),
                                                     1.0 / stretch.z())};
    }

    return true;
  }

private:
  vec3 stretch;
};

#endif // !MATERIAL_H
