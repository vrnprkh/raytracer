#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "rtweekend.h"
#include "sphere.h"
#include "vec3.h"
#include <memory>

int main() {
  hittable_list world;

  auto material_ground = make_shared<lambertian>(color(0.6, 0.6, 0.3));
  auto material_center = make_shared<metal>(color(0.1, 0.2, 0.5));
  auto material_left = make_shared<metal>(color(0.5, 0.8, 0.3));
  auto material_right = make_shared<metal>(color(0.8, 0.6, 0.4));

  world.add(
      make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

  // Camera
  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 1920;
  cam.samples_per_pixel = 500;
  cam.max_depth = 30;
  cam.render(world);
}
