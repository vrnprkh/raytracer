#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "ray.h"
#include "rtweekend.h"
#include "sphere.h"
#include "vec3.h"
#include <iostream>
#include <memory>

// Sphere early
color ray_color(const ray &r, hittable_list &world) {
  hit_record rec;
  if (world.hit(r, interval(0, infinity), rec)) {
    return 0.5 * (rec.normal + color(1, 1, 1));
  }

  // blending
  vec3 unit_direction = unit_vector(r.direction());
  // half above since we want y from -1 to 1
  double a = 0.5 * (unit_direction.y() + 1.0);
  // blend from whites to blues
  // blended value = (1-a)*start +a*end
  return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
  // Image

  double aspect_ratio = 16.0 / 9.0;
  int image_width = 400;
  int image_height = int(image_width / aspect_ratio);
  image_height = (image_height < 1) ? 1 : image_height;

  // world

  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

  // Camera

  double viewport_height = 2.0; // arbritary
  double viewport_width =
      viewport_height * (double(image_width) / image_height);
  // distance from viewport to camera
  double focal_length = 1.0;
  vec3 camera_center = point3(0, 0, 0);

  // vectors accros horiztonal (l->r) and down the vectrical (u->d) viewport
  vec3 viewport_u = vec3(viewport_width, 0, 0);
  vec3 viewport_v = vec3(0, -viewport_height, 0); // 3d coordinate convention
  vec3 pixel_delta_u = viewport_u / image_width;
  vec3 pixel_delta_v = viewport_v / image_height;

  // top left corner of the viewport (not centered on the pixel)
  point3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) -
                               viewport_u / 2 - viewport_v / 2;

  // center on the pixel
  point3 pixel00_location =
      viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

  // render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = 0; j < image_height; j++) {
    std::clog << "\rScanlines remaining: " << (image_height - j) << std::flush;
    for (int i = 0; i < image_width; i++) {
      point3 pixel_center =
          pixel00_location + (i * pixel_delta_u) + (j * pixel_delta_v);
      vec3 ray_direction = pixel_center - camera_center;
      ray r{camera_center, ray_direction};

      color pixel_color = ray_color(r, world);
      write_color(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone.                   \n";
}
