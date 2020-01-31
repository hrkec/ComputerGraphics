#include <iostream>
#include <fstream>
#include <cfloat>
#include <chrono>
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "random.h"
#include "material.h"

double hit_sphere(const vec3& center, double radius, const ray& r){
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = 2.0 * dot(oc, r.direction());
    double c = dot(oc, oc) - radius*radius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant) ) / (2. * a);
    }
}

vec3 color(const ray& r, hittable *world, int depth){
    hit_record record;
    if(world->hit(r, 0.01, FLT_MAX, record)){
        ray scattered;
        vec3 attenuation;
        if(depth < 100 && record.mat->scatter(r, record, attenuation, scattered)){
            return attenuation * color(scattered, world, depth + 1);
        } else {
            return vec3(0, 0, 0);
        }
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

double my_rand(){
    return ((double)(rand()%100001) / 100000);
}

hittable *random_world(){
    int n = 500;
    hittable **spheres = new hittable*[n];
    spheres[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.4, 0.5, 0.6)));
    int i = 1;
    for(int a = -11; a < 11; a++){
        for(int b = -11; b < 11; b++){
            double choose_material = random_double();
            double radius;
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if((center-vec3(3.5, 0.2, 0)).length() > 0.9){
                if(choose_material < 0.8){
                    radius = random_double() / 10 + 0.2;
                    vec3 center2(center.x(), radius, center.z());
                    spheres[i++] = new sphere(center2, radius, new lambertian(vec3(
                                    my_rand(),
                                    my_rand(),
                                    my_rand())));
                } else if (choose_material < 0.95){
                    radius = random_double() / 10 + 0.2;
                    vec3 center2(center.x(), radius, center.z());
                    spheres[i++] = new sphere(center2, radius, new metal(vec3(
                                    0.5*(1 + my_rand()),
                                    0.5*(1 + my_rand()),
                                    0.5*(1 + my_rand())),
                                            0.5*my_rand()));
                } else {
                    spheres[i++] = new sphere(center, 0.2, new glass(1.5));
                }
            }
        }
    }

    spheres[i++] = new sphere(vec3(-4, 1, 0), 1, new lambertian(vec3(0.9, 0.05, 0.05)));
    spheres[i++] = new sphere(vec3(3.5, 1, 0), 1.0, new metal(vec3(0.6, 0.5, 0.4), 0.0));
    spheres[i++] = new sphere(vec3(0, 1, 0), 1.0, new glass(1.6));

    return new hittable_list(spheres, i);
}

int main(int argc, char** argv) {
    int nx = std::atoi(argv[1]);
    int ny = std::atoi(argv[2]);
    int ns = std::atoi(argv[3]);

    srand(time(NULL));

    std::ofstream MyFile("awesome.ppm");
    MyFile << "P3\n" << nx << " " << ny << "\n255\n";

    hittable *world = random_world();

    /*hittable *list[5];
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.1, 0.1)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new glass(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), -0.45, new glass(1.5));

    hittable *world = new hittable_list(list, 5);*/

    vec3 lookfrom(11,2, 10);
    vec3 lookat(0,0,0);

    camera cam(lookfrom, lookat, vec3(0, 1, 0), 30, double(nx) / double(ny));

    auto start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);

    std::cout << "resolution " << nx << " x " << ny << "\n";
    std::cout << "number of rays: " << ns << "\n";
    std::cout << "began at " << std::ctime(&start_time) << "\n";

    for(int j = ny - 1; j >= 0; j--){
        for(int i = 0; i < nx; i++){
            vec3 col(0, 0, 0);

            for(int s = 0; s < ns; s++){
                double u = double(i + random_double()) / double(nx);
                double v = double(j + random_double()) / double(ny);
                col += color(cam.get_ray(u, v), world, 0);
            }

            col /= double(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

            int ir = int(255.99*col.r());
            int ig = int(255.99*col.g());
            int ib = int(255.99*col.b());

            MyFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    MyFile.close();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}
