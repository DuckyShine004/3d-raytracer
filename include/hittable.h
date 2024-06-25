#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// Abstract class for storing surfaces that a ray might hit.
class hit_record {
  public:
    point3 p;
    vec3 normal;
    double t;
    bool is_front_face;

    // Set the surface normal to "opposite" direction of the ray direction
    void set_face_normal(const ray &r, const vec3 &outward_normal) {
        is_front_face = dot(r.direction(), outward_normal) < 0;
        normal = is_front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rec) const = 0;
};

#endif
