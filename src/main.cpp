#include "utility.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

// Linear interpolate color of ray
color ray_color(const ray &r, const hittable &world) {
    hit_record rec;

    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
    // Using an aspect ratio of 16:9 for the virtual viewport
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Ensure that the height of the image is at least 1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // World/scene
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Define the camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0, 0, 0);

    // Get the horizontal and vertical viewport (directional)
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Get the unit horizontal and vertical delta vectors (directional)
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Get the location of the topleft pixel
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    fs::path file_path = "snapshots/ray-sphere-intersection-gradient-world.ppm";
    fs::create_directories(file_path.parent_path());
    std::ofstream out_file(file_path);

    if (!out_file) {
        std::cerr << "Error: Cannot open the output file." << std::endl;
        return 1;
    }

    out_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Start rendering
    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(out_file, pixel_color);
        }
    }

    out_file.close();
    std::clog << "\rDone.                 \n";
}
