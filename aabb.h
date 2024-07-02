#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "ray.h"
#include "vec3.h"

class aabb {
public:
  interval x, y, z;

  aabb() {}
  aabb(const interval &x, const interval &y, const interval &z)
      : x{x}, y{y}, z{z} {}
  aabb(const point3 &a, const point3 &b) {
    x = a[0] < b[0] ? interval(a[0], b[0]) : interval(b[0], a[0]);
    y = a[1] < b[1] ? interval(a[1], b[1]) : interval(b[1], a[1]);
    z = a[2] < b[2] ? interval(a[2], b[2]) : interval(b[2], a[2]);
  }

  aabb(const aabb &box1, const aabb &box2) {
    x = interval{box1.x, box2.x};
    y = interval{box1.y, box2.y};
    z = interval{box1.z, box2.z};
  }

  const interval &axis_interval(int n) const {
    if (n == 1) {
      return y;
    }
    if (n == 2) {
      return z;
    }
    return x;
  }

  bool hit(const ray &r, interval ray_t) const {
    const point3 &ray_orgin = r.origin();
    const vec3 &ray_dir = r.direction();

    for (int axis = 0; axis < 3; axis++) {
      const interval &ax = axis_interval(axis);
      const double adinv = 1.0 / ray_dir[axis];

      auto t0 = (ax.min - ray_orgin[axis]) * adinv;
      auto t1 = (ax.max - ray_orgin[axis]) * adinv;

      if (t0 < t1) {
        if (t0 > ray_t.min) {
          ray_t.min = t0;
        }
        if (t1 < ray_t.max) {
          ray_t.max = t1;
        }
      } else {
        if (t1 > ray_t.min) {
          ray_t.min = t1;
        }
        if (t0 < ray_t.max) {
          ray_t.max = t0;
        }
      }
      if (ray_t.max <= ray_t.min) {
        return false;
      }
    }
    return true;
  }

  int longest_axis() {
    if (x.size() > y.size()) {
      return x.size() > z.size() ? 0 : 2;
    } else {
      return y.size() > z.size() ? 1 : 2;
    }
  }

  static const aabb empty, universe;
};

const aabb aabb::empty =
    aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe =
    aabb(interval::universe, interval::universe, interval::universe);
#endif // !AABB_H
