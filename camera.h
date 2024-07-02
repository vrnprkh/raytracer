#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"
#include <iostream>
#include <ostream>
class camera {
public:
  double aspect_ratio = 1.0;
  int image_width = 100;
  int samples_per_pixel = 10;
  int max_depth = 10;

  double vfov = 90;         // vertical fov
  point3 lookfrom{0, 0, 0}; // Point camera is looking from
  point3 lookat{0, 0, -1};  // Point camera is looking at
  vec3 vup{0, 1, 0};        // Camera-relative "up" direction

  double defocus_angle = 0;
  double focus_dist = 10;

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
          pixel_color += ray_color(r, max_depth, world);
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

  vec3 u, v, w; // Camera frame basis vectors
  vec3 defocus_disk_u;
  vec3 defocus_disk_v;
  void initialize() {
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    center = lookfrom;
    // distance from viewport to camera

    auto theta = deg_to_rad(vfov);
    auto h = tan(theta / 2);

    double viewport_height = 2 * h * focus_dist;
    double viewport_width =
        viewport_height * (double(image_width) / image_height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // vectors accros horiztonal (l->r) and down the vectrical (u->d) viewport
    vec3 viewport_u = viewport_width * u;
    vec3 viewport_v = viewport_height * -v;
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // top left corner of the viewport (not centered on the pixel)

    auto viewport_upper_left =
        center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;

    // center on the pixel
    pixel00_location =
        viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    pixel_sample_scale = 1.0 / samples_per_pixel;

    auto defocus_radius = focus_dist * tan(deg_to_rad(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
  }

  color ray_color(const ray &r, int depth, const hittable &world) const {
    hit_record rec;
    if (depth <= 0) {
      return color{0, 0, 0};
    }
    if (world.hit(r, interval(0.001, infinity), rec)) {
      ray scattered;
      color attenuation;
      if (rec.mat->scatter(r, rec, attenuation, scattered)) {
        return attenuation * ray_color(scattered, depth - 1, world);
      }
      return color{0, 0, 0};
    }

    // background
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
    auto ray_origin = (defocus_angle <= 0) ? center : defocus_sample();
    auto ray_dir = pixel_sample - ray_origin;
    auto ray_time = random_double(0, 1);
    return ray{ray_origin, ray_dir, ray_time};
  }
  vec3 sample_square() const {
    return vec3{random_double() - 0.5, random_double() - .5, 0};
  }

  point3 defocus_sample() const {
    auto p = random_in_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
  }
};

#endif // !CAMERA_H
