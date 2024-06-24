#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
// using
using std::make_shared;
using std::shared_ptr;
using std::sqrt;
using std::unique_ptr;

// constnats
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// util
inline double deg_to_rad(double degrees) { return degrees * pi / 180.0; }

inline double random_double() { return rand() / (RAND_MAX + 1.0); }
inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}
#endif
