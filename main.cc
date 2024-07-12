#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "rtweekend.h"
#include "sphere.h"
#include "vec3.h"

#include "lib/raylib.h"

#include <memory>
#include <mutex>
#include <tuple>

void bouncingSpheres() {
  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          auto center2 = center + vec3(0, random_double(0, 0), 0);
          world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  // create bvh
  world = hittable_list(make_shared<bvh_node>(world));

  //  camera
  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 30;

  cam.vfov = 20;
  cam.lookfrom = point3(13, 2, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;

  cam.render(world);
}

void foo() {

  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          auto center2 = center + vec3(0, random_double(0, 0), 0);
          world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  // create bvh
  world = hittable_list(make_shared<bvh_node>(world));

  //  camera
  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 1200;
  cam.samples_per_pixel = 1000;
  cam.max_depth = 30;

  cam.vfov = 20;
  cam.lookfrom = point3(13, 2, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;

  cam.manualinit();
  // default output vector
  std::vector<std::vector<color>> outputVector;
  for (int j = 0; j < cam.getImageHeight(); j++) {
    outputVector.push_back(std::vector<color>());

    for (int i = 0; i < cam.image_width; i++) {
      outputVector[j].push_back(color(0, 0, 0));
    }
  }

  int sampleNum = 0;
  int h = outputVector.size();
  int w = outputVector[0].size();
  std::mutex mtx;

  std::thread camThread =
      std::thread([&cam, &world, &outputVector, &mtx, &sampleNum]() {
        cam.renderToVector(world, outputVector, mtx, sampleNum);
      });
  auto valClamp = interval(0, 0.9999);
  InitWindow(w, h, "FOO");
  BeginDrawing();
  EndDrawing();
  while (!WindowShouldClose()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::lock_guard<std::mutex> lock(mtx);
    if (sampleNum == 0) {
      continue;
    }
    std::cout << "main " << sampleNum << std::endl;
    BeginDrawing();
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        DrawPixel(
            x, y,
            Color{
                (unsigned char)(256 * valClamp.clamp(linear_to_gamma(
                                          outputVector[y][x].x() / sampleNum))),
                (unsigned char)(256 * valClamp.clamp(linear_to_gamma(
                                          outputVector[y][x].y() / sampleNum))),
                (unsigned char)(256 * valClamp.clamp(linear_to_gamma(
                                          outputVector[y][x].z() / sampleNum))),
                255});
      }
    }
    EndDrawing();
    ClearBackground(Color{0, 0, 0, 0});
  }
  CloseWindow();
  camThread.join();
}

int main() {
  switch (3) {
  case 1:
    bouncingSpheres();
    break;
  case 2:
    break;

  case 3:
    foo();
  }
}
