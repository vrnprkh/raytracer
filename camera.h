#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"
#include <iostream>
#include <ostream>
class camera {
public:
  double aspect_ratio = 1.0;
  int image_width = 100;
  int samples_per_pixel = 10;
  void render(const hittable &world) {
    initialize();
    // render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
      std::clog << "\rScanlines remaining: " << (image_height - j)
                << std::flush;
      for (int i = 0; i < image_width; i++) {
        color pixel_color{0, 0, 0};
        for (int sample = 0; sample < samples_per_pixel; sample++) {
          ray r = get_ray(i, j);
          pixel_color += ray_color(r, world);
        }
        write_color(std::cout, pixel_color * pixel_sample_scale);
      }
    }
    std::clog << "\rDone.                   \n";
  }

private:
  int image_height;
  point3 center;
  vec3 pixel_delta_v;
  vec3 pixel_delta_u;
  point3 pixel00_location;
  double pixel_sample_scale;
  void initialize() {
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    double viewport_height = 2.0; // arbritary
    double viewport_width =
        viewport_height * (double(image_width) / image_height);
    // distance from viewport to camera
    double focal_length = 1.0;
    center = point3(0, 0, 0);

    // vectors accros horiztonal (l->r) and down the vectrical (u->d) viewport
    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0); // 3d coordinate convention
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // top left corner of the viewport (not centered on the pixel)
    point3 viewport_upper_left =
        center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

    // center on the pixel
    pixel00_location =
        viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    pixel_sample_scale = 1.0 / samples_per_pixel;
  }

  color ray_color(const ray &r, const hittable &world) const {
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

  ray get_ray(int i, int j) const {
    auto offset = sample_square();
    auto pixel_sample = pixel00_location + ((i + offset.x()) * pixel_delta_u) +
                        ((j + offset.y()) * pixel_delta_v);
    auto ray_origin = center;
    auto ray_dir = pixel_sample - ray_origin;
    return ray{ray_origin, ray_dir};
  }
  vec3 sample_square() const {
    return vec3{random_double() - 0.5, random_double() - .5, 0};
  }
};

#endif // !CAMERA_H
