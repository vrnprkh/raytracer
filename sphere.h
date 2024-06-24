#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include <memory>

class sphere : public hittable {
private:
  point3 center;
  double radius;
  std::shared_ptr<material> mat;

public:
  sphere(const point3 &center, double radius, std::shared_ptr<material> mat)
      : center{center}, radius{fmax(0, radius)}, mat{mat} {

    // TODO: init mat
  }

  bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc); // b = -2h
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0) {
      return false;
    }

    auto sqrtd = sqrt(discriminant);
    // check bounds for both roots
    // - discriminant
    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      // + discriminant
      root = (h + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        return false;
      }
    }
    // update hit hit_record
    rec.t = root;
    rec.p = vec3(r.at(rec.t));
    rec.mat = mat;
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    return true;
  }
};
#endif // !SPHERE_H
