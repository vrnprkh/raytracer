#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
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

// common headers
#include "color.h"
#include "ray.h"
#include "vec3.h"
#endif
