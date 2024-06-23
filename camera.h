#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "ray.h"
class camera {
public:
  double aspect_ratio = 1.0;
  int image_width = 100;

  void render(const hittable &world) {
    initialize();
    // render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
      std::clog << "\rScanlines remaining: " << (image_height - j)
                << std::flush;
      for (int i = 0; i < image_width; i++) {
        point3 pixel_center =
            pixel00_location + (i * pixel_delta_u) + (j * pixel_delta_v);
        vec3 ray_direction = pixel_center - center;
        ray r{center, ray_direction};

        color pixel_color = ray_color(r, world);
        write_color(std::cout, pixel_color);
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
};

#endif // !CAMERA_H
