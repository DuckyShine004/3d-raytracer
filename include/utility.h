#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>

// C++ Std Usings
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Namespaces
namespace fs = std::filesystem;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Return a random number in the range [0,1)
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

// Retrun a random number in the range [min,max)
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

// Common Headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
