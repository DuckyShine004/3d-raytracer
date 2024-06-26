#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"
#include "hittable.h"

class camera {
  public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10; // Count of random samples per pixel (for anti-aliasing)
    int max_depth = 10;         // Number of ray bounces

    void render(const hittable &world) {
        initialize();

        fs::path file_path = "snapshots/ray-tracing-diffuse.ppm";
        fs::create_directories(file_path.parent_path());
        std::ofstream out_file(file_path);

        if (!out_file) {
            std::cerr << "Error: Cannot open the output file." << std::endl;
            return;
        }

        out_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        // Start ray tracing
        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);

                // Start anti-aliasing by sampling neighbor pixels and applying filter
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(out_file, pixel_samples_scale * pixel_color);
            }
        }

        out_file.close();
        std::clog << "\rDone.                 \n";
    }

  private:
    int image_height;           // Rendered image height
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples
    point3 center;              // Camera center
    point3 pixel00_loc;         // Location of pixel 0, 0
    vec3 pixel_delta_u;         // Offset to pixel to the right
    vec3 pixel_delta_v;         // Offset to pixel below

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = point3(0, 0, 0);

        // Define camera parameters
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // Get the horizonral and veritcal viewport vectors (directional)
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Get the unit horizontal and vertical delta vectors (directional)
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Get the location of the topleft pixel
        auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    // Recursively reflect rays and surface color and brightness
    color ray_color(const ray &r, int depth, const hittable &world) const {
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        hit_record rec;

        // If the ray intersects with any surface, the ray is scattered
        // uniformly in all directions (diffuse reflections). We then
        // have to recursively check if the reflected ray intersects with
        // other surfaces in the scene
        if (world.hit(r, interval(0, infinity), rec)) {
            vec3 direction = random_on_hemisphere(rec.normal);

            return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);

        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    // Get the ray to the random sample point
    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square
    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }
};

#endif
