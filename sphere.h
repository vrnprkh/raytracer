#ifndef SPHERE_H
#define SPHERE_H
#include "aabb.h"
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include <memory>

class sphere : public hittable {
private:
  point3 center1;
  double radius;
  std::shared_ptr<material> mat;
  vec3 center_vec;
  bool is_moving;
  aabb bbox;

public:
  // stationary sphere
  sphere(const point3 &center, double radius, std::shared_ptr<material> mat)
      : center1{center}, radius{fmax(0, radius)}, mat{mat}, is_moving{false} {
    auto radiusVec = vec3{radius, radius, radius};
    bbox = aabb(center1 - radiusVec, center1 + radiusVec);
  }
  // moving sphere
  sphere(const point3 &center1, const point3 &center2, double radius,
         std::shared_ptr<material> mat)
      : center1{center1}, radius{fmax(0, radius)}, mat{mat}, is_moving{true} {
    auto radiusVec = vec3{radius, radius, radius};
    aabb bbox1 = aabb(center1 - radiusVec, center1 + radiusVec);
    aabb bbox2 = aabb(center2 - radiusVec, center2 + radiusVec);
    bbox = aabb(bbox1, bbox2);
    center_vec = center2 - center1;
  }
  point3 sphere_center(double time) const {
    return center1 + time * center_vec;
  }
  aabb bounding_box() const override { return bbox; }

  bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
    vec3 center = is_moving ? sphere_center(r.time()) : center1;
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
