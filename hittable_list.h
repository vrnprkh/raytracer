#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include "aabb.h"
#include "hittable.h"
#include "interval.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::unique_ptr;

class hittable_list : public hittable {
private:
  aabb bbox;

public:
  std::vector<shared_ptr<hittable>> objects;

  hittable_list() {}
  hittable_list(shared_ptr<hittable> obj) { add(obj); }

  void clear() { objects.clear(); }
  void add(shared_ptr<hittable> obj) {
    objects.push_back(obj);
    bbox = aabb(bbox, obj->bounding_box());
  }

  bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_t.max;
    for (auto &obj : objects) {
      if (obj->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }
    }
    return hit_anything;
  }

  aabb bounding_box() const override { return bbox; }
};

#endif
