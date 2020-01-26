#include <glm/glm.hpp>
#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <glm/ext/scalar_constants.inl>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <functional>

class SaveImage {
public:
    static void framebufferToJPG(const int width, const int height, const std::vector<glm::vec3>& fb) {
        std::vector<char> image(width * height * 3);
        for (size_t i = 0; i < height * width; ++i) {
            image[i * 3] = static_cast<char>(fb[i].x * 255);
            image[(i * 3) + 1] = static_cast<char>(fb[i].y * 255);
            image[(i * 3) + 2] = static_cast<char>(fb[i].z * 255);
        }
        stbi_write_jpg("../assets/raytracing/01_SaveImage.jpg", 1024, 768, 3, image.data(), 50);
    }

    static void render() {
        const int width = 1024;
        const int height = 768;
        std::vector<glm::vec3> framebuffer(width * height);
        for (size_t j = 0; j < height; j++) {
            for (size_t i = 0; i < width; i++) {
                framebuffer[i + j * width] = glm::vec3(j / float(height), i / float(width), 0);
            }
        }
        framebufferToJPG(width, height, framebuffer);
    }

    static void main() {
        render();
    }
};

class RayTracing {
public:
    struct Sphere {
        glm::vec3 center;
        float radius;
        Sphere(const glm::vec3& c, const float& r) : center(c), radius(r) {}
        bool ray_intersect(const glm::vec3& orig, const glm::vec3& dir, float& t0) const {
            glm::vec3 L = center - orig;
            float tca = glm::dot(L, dir);
            float d2 = glm::dot(L, L) - glm::dot(tca, tca);
            if (d2 > radius* radius) return false;
            float thc = sqrtf(radius * radius - d2);
            t0 = tca - thc;
            float t1 = tca + thc;
            if (t0 < 0) t0 = t1;
            if (t0 < 0) return false;
            return true;
        }
    };

    static void framebufferToJPG(const int width, const int height, const std::vector<glm::vec3>& fb) {
        std::vector<char> image(width * height * 3);
        for (size_t i = 0; i < height * width; ++i) {
            image[i * 3] = static_cast<char>(fb[i].x * 255);
            image[(i * 3) + 1] = static_cast<char>(fb[i].y * 255);
            image[(i * 3) + 2] = static_cast<char>(fb[i].z * 255);
        }
        stbi_write_jpg("../assets/raytracing/02_RayTracing.jpg", 1024, 768, 3, image.data(), 50);
    }

    static glm::vec3 cast_ray(const glm::vec3& orig, const glm::vec3& dir, const Sphere& sphere) {
        float sphere_dist = std::numeric_limits<float>::max();
        if (!sphere.ray_intersect(orig, dir, sphere_dist)) {
            return glm::vec3(0.2f, 0.2f, 0.2f); // background color
        }
        return glm::vec3(0.4f, 0.4f, 0.3f);
    }

    static void render(const Sphere& sphere) {
        const int width = 1024;
        const int height = 768;
        const int fov = glm::pi<float>() / 2.;
        std::vector<glm::vec3> framebuffer(width * height);
        for (size_t j = 0; j < height; j++) {
            for (size_t i = 0; i < width; i++) {
                float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
                float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
                glm::vec3 dir = glm::normalize(glm::vec3(x, y, -1));
                framebuffer[i + j * width] = cast_ray(glm::vec3(0, 0, 0), dir, sphere);
            }
        }
        framebufferToJPG(width, height, framebuffer);
    }

    static void main() {
        Sphere sphere(glm::vec3(-3.0f, 0.0f, -16.0f), 2.0f);
        render(sphere);
    }
};

class MoreSpheres {
public:
    struct Material {
        Material(const glm::vec3 color) : diffuse_color(color) {}
        Material() : diffuse_color() {}
        glm::vec3 diffuse_color;
    };

    struct Sphere {
        glm::vec3 center;
        float radius;
        Material material;
        Sphere(const glm::vec3& c, const float& r, const Material& m) : center(c), radius(r), material(m) {}
        bool ray_intersect(const glm::vec3& orig, const glm::vec3& dir, float& t0) const {
            glm::vec3 L = center - orig;
            float tca = glm::dot(L, dir);
            float d2 = glm::dot(L, L) - glm::dot(tca, tca);
            if (d2 > radius* radius) return false;
            float thc = sqrtf(radius * radius - d2);
            t0 = tca - thc;
            float t1 = tca + thc;
            if (t0 < 0) t0 = t1;
            if (t0 < 0) return false;
            return true;
        }
    };

    static void framebufferToJPG(const int width, const int height, const std::vector<glm::vec3>& fb) {
        std::vector<char> image(width * height * 3);
        for (size_t i = 0; i < height * width; ++i) {
            image[i * 3] = static_cast<char>(fb[i].x * 255);
            image[(i * 3) + 1] = static_cast<char>(fb[i].y * 255);
            image[(i * 3) + 2] = static_cast<char>(fb[i].z * 255);
        }
        stbi_write_jpg("../assets/raytracing/03_MoreSpheres.jpg", 1024, 768, 3, image.data(), 50);
    }

    static bool scene_intersect(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres, glm::
        vec3& hit, glm::vec3& N, Material& material) {
        float spheres_dist = std::numeric_limits<float>::max();
        for (size_t i = 0; i < spheres.size(); i++) {
            float dist_i;
            if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
                spheres_dist = dist_i;
                hit = orig + dir * dist_i;
                N = glm::normalize(hit - spheres[i].center);
                material = spheres[i].material;
            }
        }
        return spheres_dist < 1000;
    }

    static glm::vec3 cast_ray(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres) {
        glm::vec3 point, N;
        Material material;
        if (!scene_intersect(orig, dir, spheres, point, N, material)) {
            return glm::vec3(0.2, 0.2, 0.2); // background color
        }
        return material.diffuse_color;
    }

    static void render(const std::vector<Sphere>& spheres) {
        const int width = 1024;
        const int height = 768;
        const int fov = glm::pi<float>() / 2.;
        std::vector<glm::vec3> framebuffer(width * height);
        for (size_t j = 0; j < height; j++) {
            for (size_t i = 0; i < width; i++) {
                float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
                float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
                glm::vec3 dir = glm::normalize(glm::vec3(x, y, -1));
                framebuffer[i + j * width] = cast_ray(glm::vec3(0, 0, 0), dir, spheres);
            }
        }
        framebufferToJPG(width, height, framebuffer);
    }

    static void main() {
        Material ivory(glm::vec3(0.4, 0.4, 0.3));
        Material rubber(glm::vec3(0.1, 0.3, 0.1));
        std::vector<Sphere> spheres;
        spheres.push_back(Sphere(glm::vec3(-3, 0, -16), 2, ivory));
        spheres.push_back(Sphere(glm::vec3(-1.0, -1.5, -12), 2, rubber));
        spheres.push_back(Sphere(glm::vec3(1.5, -0.5, -18), 3, rubber));
        spheres.push_back(Sphere(glm::vec3(7, 5, -18), 4, ivory));
        render(spheres);
    }
};

class Lighting {
public:
    struct Light {
        Light(const glm::vec3& p, const float& i) : position(p), intensity(i) {}
        glm::vec3 position;
        float intensity;
    };

    struct Material {
        Material(const glm::vec3 color) : diffuse_color(color) {}
        Material() : diffuse_color() {}
        glm::vec3 diffuse_color;
    };

    struct Sphere {
        glm::vec3 center;
        float radius;
        Material material;
        Sphere(const glm::vec3& c, const float& r, const Material& m) : center(c), radius(r), material(m) {}
        bool ray_intersect(const glm::vec3& orig, const glm::vec3& dir, float& t0) const {
            glm::vec3 L = center - orig;
            float tca = glm::dot(L, dir);
            float d2 = glm::dot(L, L) - glm::dot(tca, tca);
            if (d2 > radius* radius) return false;
            float thc = sqrtf(radius * radius - d2);
            t0 = tca - thc;
            float t1 = tca + thc;
            if (t0 < 0) t0 = t1;
            if (t0 < 0) return false;
            return true;
        }
    };

    static void framebufferToJPG(const int width, const int height, const std::vector<glm::vec3>& fb) {
        std::vector<char> image(width * height * 3);
        for (size_t i = 0; i < height * width; ++i) {
            image[i * 3] = static_cast<char>(fb[i].x * 255);
            image[(i * 3) + 1] = static_cast<char>(fb[i].y * 255);
            image[(i * 3) + 2] = static_cast<char>(fb[i].z * 255);
        }
        stbi_write_jpg("../assets/raytracing/04_Lighting.jpg", 1024, 768, 3, image.data(), 50);
    }

    static bool scene_intersect(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres, glm::
        vec3& hit, glm::vec3& N, Material& material) {
        float spheres_dist = std::numeric_limits<float>::max();
        for (size_t i = 0; i < spheres.size(); i++) {
            float dist_i;
            if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
                spheres_dist = dist_i;
                hit = orig + dir * dist_i;
                N = glm::normalize(hit - spheres[i].center);
                material = spheres[i].material;
            }
        }
        return spheres_dist < 1000;
    }

    static glm::vec3 cast_ray(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres, const
        std::vector<Light>& lights) {
        glm::vec3 point, N;
        Material material;
        if (!scene_intersect(orig, dir, spheres, point, N, material)) {
            return glm::vec3(0.2, 0.2, 0.2); // background color
        }
        float diffuse_light_intensity = 0;
        for (size_t i = 0; i < lights.size(); i++) {
            glm::vec3 light_dir = glm::normalize(lights[i].position - point);
            diffuse_light_intensity += lights[i].intensity * std::max(0.f, glm::dot(light_dir, N));
        }
        return material.diffuse_color * diffuse_light_intensity;
    }

    static void render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
        const int width = 1024;
        const int height = 768;
        const int fov = glm::pi<float>() / 2.;
        std::vector<glm::vec3> framebuffer(width * height);
        for (size_t j = 0; j < height; j++) {
            for (size_t i = 0; i < width; i++) {
                float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
                float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
                glm::vec3 dir = glm::normalize(glm::vec3(x, y, -1));
                framebuffer[i + j * width] = cast_ray(glm::vec3(0, 0, 0), dir, spheres, lights);
            }
        }
        framebufferToJPG(width, height, framebuffer);
    }

    static void main() {
        Material ivory(glm::vec3(0.4, 0.4, 0.3));
        Material rubber(glm::vec3(0.1, 0.3, 0.1));
        std::vector<Sphere> spheres;
        spheres.push_back(Sphere(glm::vec3(-3, 0, -16), 2, ivory));
        spheres.push_back(Sphere(glm::vec3(-1.0, -1.5, -12), 2, rubber));
        spheres.push_back(Sphere(glm::vec3(1.5, -0.5, -18), 3, rubber));
        spheres.push_back(Sphere(glm::vec3(7, 5, -18), 4, ivory));
        std::vector<Light> lights;
        lights.push_back(Light(glm::vec3(-20, 20, 20), 1.5));
        render(spheres, lights);
    }
};

class Specular {
public:
    struct Light {
        Light(const glm::vec3& p, const float& i) : position(p), intensity(i) {}
        glm::vec3 position;
        float intensity;
    };

    struct Material {
        Material(const glm::vec2& a, const glm::vec3& color, const float& spec) : albedo(a), diffuse_color(color ), specular_exponent(spec) {}
        Material() : albedo(1, 0), diffuse_color(), specular_exponent() {}
        glm::vec2 albedo;
        glm::vec3 diffuse_color;
        float specular_exponent;
    };

    struct Sphere {
        glm::vec3 center;
        float radius;
        Material material;
        Sphere(const glm::vec3& c, const float& r, const Material& m) : center(c), radius(r), material(m) {}
        bool ray_intersect(const glm::vec3& orig, const glm::vec3& dir, float& t0) const {
            glm::vec3 L = center - orig;
            float tca = glm::dot(L, dir);
            float d2 = glm::dot(L, L) - glm::dot(tca, tca);
            if (d2 > radius* radius) return false;
            float thc = sqrtf(radius * radius - d2);
            t0 = tca - thc;
            float t1 = tca + thc;
            if (t0 < 0) t0 = t1;
            if (t0 < 0) return false;
            return true;
        }
    };

    static void framebufferToJPG(const int width, const int height, std::vector<glm::vec3> fb) {
        std::vector<char> image(width * height * 3);
        for (size_t i = 0; i < height * width; ++i) {
            glm::vec3& c = fb[i];
            float max = std::max(c[0], std::max(c[1], c[2]));
            if (max > 1) c = c * (1.0f / max);
            image[i * 3] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].x * 255));
            image[(i * 3) + 1] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].y * 255));
            image[(i * 3) + 2] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].z * 255));
        }
        stbi_write_jpg("../assets/raytracing/05_Specular.jpg", 1024, 768, 3, image.data(), 50);
    }

    static bool scene_intersect(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres, glm::
        vec3& hit, glm::vec3& N, Material& material) {
        float spheres_dist = std::numeric_limits<float>::max();
        for (size_t i = 0; i < spheres.size(); i++) {
            float dist_i;
            if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
                spheres_dist = dist_i;
                hit = orig + dir * dist_i;
                N = glm::normalize(hit - spheres[i].center);
                material = spheres[i].material;
            }
        }
        return spheres_dist < 1000;
    }

    static glm::vec3 reflect(const glm::vec3& I, const glm::vec3& N) {
        return I - N * 2.f * glm::dot(I, N);
    }

    static glm::vec3 cast_ray(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres, const
        std::vector<Light>& lights) {
        glm::vec3 point, N;
        Material material;
        if (!scene_intersect(orig, dir, spheres, point, N, material)) {
            return glm::vec3(0.2, 0.2, 0.2); // background color
        }
        float diffuse_light_intensity = 0, specular_light_intensity = 0;
        for (size_t i = 0; i < lights.size(); i++) {
            glm::vec3 light_dir = glm::normalize(lights[i].position - point);
            diffuse_light_intensity += lights[i].intensity * std::max(0.f, glm::dot(light_dir, N));
            specular_light_intensity += powf(std::max(0.f, glm::dot(-reflect(-light_dir, N), dir)), material.specular_exponent) * lights[i].intensity;
        }
        return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + glm::vec3(1., 1., 1.) *
            specular_light_intensity * material.albedo[1];
    }

    static void render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
        const int width = 1024;
        const int height = 768;
        const int fov = glm::pi<float>() / 2.;
        std::vector<glm::vec3> framebuffer(width * height);
        for (size_t j = 0; j < height; j++) {
            for (size_t i = 0; i < width; i++) {
                float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
                float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
                glm::vec3 dir = glm::normalize(glm::vec3(x, y, -1));
                framebuffer[i + j * width] = cast_ray(glm::vec3(0, 0, 0), dir, spheres, lights);
            }
        }
        framebufferToJPG(width, height, framebuffer);
    }

    static void main() {
        Material ivory(glm::vec2(0.6, 0.3), glm::vec3(0.4, 0.4, 0.3), 50.);
        Material rubber(glm::vec2(0.2, 0.8), glm::vec3(0.1, 0.3, 0.1), 75.);
        std::vector<Sphere> spheres;
        spheres.push_back(Sphere(glm::vec3(-3, 0, -16), 2, ivory));
        spheres.push_back(Sphere(glm::vec3(-1.0, -1.5, -12), 2, rubber));
        spheres.push_back(Sphere(glm::vec3(1.5, -0.5, -18), 3, rubber));
        spheres.push_back(Sphere(glm::vec3(7, 5, -18), 4, ivory));
        std::vector<Light> lights;
        lights.push_back(Light(glm::vec3(-20, 20, 20), 1.5));
        lights.push_back(Light(glm::vec3(30, 50, -25), 1.8));
        lights.push_back(Light(glm::vec3(30, 20, 30), 1.7));
        render(spheres, lights);
    }
};

class Shadows {
public:
    struct Light {
        Light(const glm::vec3& p, const float& i) : position(p), intensity(i) {}
        glm::vec3 position;
        float intensity;
    };

    struct Material {
        Material(const glm::vec2& a, const glm::vec3& color, const float& spec) : albedo(a), diffuse_color(color), specular_exponent(spec) {}
        Material() : albedo(1, 0), diffuse_color(), specular_exponent() {}
        glm::vec2 albedo;
        glm::vec3 diffuse_color;
        float specular_exponent;
    };

    struct Sphere {
        glm::vec3 center;
        float radius;
        Material material;
        Sphere(const glm::vec3& c, const float& r, const Material& m) : center(c), radius(r), material(m) {}
        bool ray_intersect(const glm::vec3& orig, const glm::vec3& dir, float& t0) const {
            glm::vec3 L = center - orig;
            float tca = glm::dot(L, dir);
            float d2 = glm::dot(L, L) - glm::dot(tca, tca);
            if (d2 > radius* radius) return false;
            float thc = sqrtf(radius * radius - d2);
            t0 = tca - thc;
            float t1 = tca + thc;
            if (t0 < 0) t0 = t1;
            if (t0 < 0) return false;
            return true;
        }
    };

    static void framebufferToJPG(const int width, const int height, std::vector<glm::vec3> fb) {
        std::vector<char> image(width * height * 3);
        for (size_t i = 0; i < height * width; ++i) {
            glm::vec3& c = fb[i];
            float max = std::max(c[0], std::max(c[1], c[2]));
            if (max > 1) c = c * (1.0f / max);
            image[i * 3] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].x * 255));
            image[(i * 3) + 1] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].y * 255));
            image[(i * 3) + 2] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].z * 255));
        }
        stbi_write_jpg("../assets/raytracing/06_Shadow.jpg", 1024, 768, 3, image.data(), 50);
    }

    static bool scene_intersect(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres, glm::
        vec3& hit, glm::vec3& N, Material& material) {
        float spheres_dist = std::numeric_limits<float>::max();
        for (size_t i = 0; i < spheres.size(); i++) {
            float dist_i;
            if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
                spheres_dist = dist_i;
                hit = orig + dir * dist_i;
                N = glm::normalize(hit - spheres[i].center);
                material = spheres[i].material;
            }
        }
        return spheres_dist < 1000;
    }

    static glm::vec3 reflect(const glm::vec3& I, const glm::vec3& N) {
        return I - N * 2.f * glm::dot(I, N);
    }

    static glm::vec3 cast_ray(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres,
        const std::vector<Light>& lights) {
        glm::vec3 point, N;
        Material material;
        if (!scene_intersect(orig, dir, spheres, point, N, material)) {
            return glm::vec3(0.2, 0.2, 0.2); // background color
        }
        float diffuse_light_intensity = 0, specular_light_intensity = 0;
        for (size_t i = 0; i < lights.size(); i++) {
            glm::vec3 light_dir = glm::normalize(lights[i].position - point);
            float light_distance = glm::length(lights[i].position - point);
            // checking if the point lies in the shadow of the lights[i]
            glm::vec3 shadow_orig = glm::dot(light_dir, N) < 0 ? point - N * 0.001f : point + N * 0.001f;
            glm::vec3 shadow_pt, shadow_N;
            Material tmpmaterial;
            if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_N, tmpmaterial) &&
                glm::length(shadow_pt - shadow_orig) < light_distance)
                continue;
            diffuse_light_intensity += lights[i].intensity * std::max(0.f, glm::dot(light_dir, N));
            specular_light_intensity +=
                powf(std::max(0.f, glm::dot(-reflect(-light_dir, N), dir)), material.specular_exponent)
                * lights[i].intensity;
        }
        return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + glm::vec3(1., 1., 1.) *
            specular_light_intensity * material.albedo[1];
    }

    static void render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
        const int width = 1024;
        const int height = 768;
        const int fov = glm::pi<float>() / 2.;
        std::vector<glm::vec3> framebuffer(width * height);
        for (size_t j = 0; j < height; j++) {
            for (size_t i = 0; i < width; i++) {
                float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
                float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
                glm::vec3 dir = glm::normalize(glm::vec3(x, y, -1));
                framebuffer[i + j * width] = cast_ray(glm::vec3(0, 0, 0), dir, spheres, lights);
            }
        }
        framebufferToJPG(width, height, framebuffer);
    }

    static void main() {
        Material ivory(glm::vec2(0.6, 0.3), glm::vec3(0.4, 0.4, 0.3), 50.);
        Material rubber(glm::vec2(0.2, 0.8), glm::vec3(0.1, 0.3, 0.1), 75.);
        std::vector<Sphere> spheres;
        spheres.push_back(Sphere(glm::vec3(-3, 0, -16), 2, ivory));
        spheres.push_back(Sphere(glm::vec3(-1.0, -1.5, -12), 2, rubber));
        spheres.push_back(Sphere(glm::vec3(1.5, -0.5, -18), 3, rubber));
        spheres.push_back(Sphere(glm::vec3(7, 5, -18), 4, ivory));
        std::vector<Light> lights;
        lights.push_back(Light(glm::vec3(-20, 20, 20), 1.5));
        lights.push_back(Light(glm::vec3(30, 50, -25), 1.8));
        lights.push_back(Light(glm::vec3(30, 20, 30), 1.7));
        render(spheres, lights);
    }
};

class Reflections {
public:
    struct Light {
        Light(const glm::vec3& p, const float& i) : position(p), intensity(i) {}
        glm::vec3 position;
        float intensity;
    };

    struct Material {
        Material(const glm::vec3& a, const glm::vec3& color, const float& spec) : albedo(a), diffuse_color(color ), specular_exponent(spec) {}
        Material() : albedo(1, 0, 0), diffuse_color(), specular_exponent() {}
        glm::vec3 albedo;
        glm::vec3 diffuse_color;
        float specular_exponent;
    };

    struct Sphere {
        glm::vec3 center;
        float radius;
        Material material;
        Sphere(const glm::vec3& c, const float& r, const Material& m) : center(c), radius(r), material(m) {}
        bool ray_intersect(const glm::vec3& orig, const glm::vec3& dir, float& t0) const {
            glm::vec3 L = center - orig;
            float tca = glm::dot(L, dir);
            float d2 = glm::dot(L, L) - glm::dot(tca, tca);
            if (d2 > radius* radius) return false;
            float thc = sqrtf(radius * radius - d2);
            t0 = tca - thc;
            float t1 = tca + thc;
            if (t0 < 0) t0 = t1;
            if (t0 < 0) return false;
            return true;
        }
    };

    static void framebufferToJPG(const int width, const int height, std::vector<glm::vec3> fb) {
        std::vector<char> image(width * height * 3);
        for (size_t i = 0; i < height * width; ++i) {
            glm::vec3& c = fb[i];
            float max = std::max(c[0], std::max(c[1], c[2]));
            if (max > 1) c = c * (1.0f / max);
            image[i * 3] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].x * 255));
            image[(i * 3) + 1] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].y * 255));
            image[(i * 3) + 2] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].z * 255));
        }
        stbi_write_jpg("../assets/raytracing/07_Reflections.jpg", 1024, 768, 3, image.data(), 50);
    }

    static bool scene_intersect(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres, glm::
        vec3& hit, glm::vec3& N, Material& material) {
        float spheres_dist = std::numeric_limits<float>::max();
        for (size_t i = 0; i < spheres.size(); i++) {
            float dist_i;
            if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
                spheres_dist = dist_i;
                hit = orig + dir * dist_i;
                N = glm::normalize(hit - spheres[i].center);
                material = spheres[i].material;
            }
        }
        return spheres_dist < 1000;
    }

    static glm::vec3 reflect(const glm::vec3& I, const glm::vec3& N) {
        return I - N * 2.f * glm::dot(I, N);
    }

    static glm::vec3 cast_ray(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres,
        const std::vector<Light>& lights, size_t depth = 0) {
        glm::vec3 point, N;
        Material material;
        if (depth > 4 || !scene_intersect(orig, dir, spheres, point, N, material)) {
            return glm::vec3(0.2, 0.2, 0.2); // background color
        }
        glm::vec3 reflect_dir = glm::normalize(reflect(dir, N));
        // offset the original point to avoid occlusion by the object itself
        glm::vec3 reflect_orig = glm::dot(reflect_dir, N) < 0 ? point - N * 0.001f : point + N * 0.001f;
        glm::vec3 reflect_color = cast_ray(reflect_orig, reflect_dir, spheres, lights, depth + 1);
        float diffuse_light_intensity = 0, specular_light_intensity = 0;
        for (size_t i = 0; i < lights.size(); i++) {
            glm::vec3 light_dir = glm::normalize(lights[i].position - point);
            float light_distance = glm::length(lights[i].position - point);
            // checking if the point lies in the shadow of the lights[i]
            glm::vec3 shadow_orig = glm::dot(light_dir, N) < 0 ? point - N * 0.001f : point + N * 0.001f;
            glm::vec3 shadow_pt, shadow_N;
            Material tmpmaterial;
            if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_N, tmpmaterial) &&
                glm::length(shadow_pt - shadow_orig) < light_distance)
                continue;
            diffuse_light_intensity += lights[i].intensity * std::max(0.f, glm::dot(light_dir, N));
            specular_light_intensity += powf(std::max(0.f, glm::dot(-reflect(-light_dir, N), dir)),
                material.specular_exponent) * lights[i].intensity;
        }
        return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + glm::vec3(1., 1., 1.) *
            specular_light_intensity * material.albedo[1] + reflect_color * material.albedo[2];
    }

    static void render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
        const int width = 1024;
        const int height = 768;
        const int fov = glm::pi<float>() / 2.;
        std::vector<glm::vec3> framebuffer(width * height);
        for (size_t j = 0; j < height; j++) {
            for (size_t i = 0; i < width; i++) {
                float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
                float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
                glm::vec3 dir = glm::normalize(glm::vec3(x, y, -1));
                framebuffer[i + j * width] = cast_ray(glm::vec3(0, 0, 0), dir, spheres, lights);
            }
        }
        framebufferToJPG(width, height, framebuffer);
    }

    static void main() {
        Material ivory(glm::vec3(0.6, 0.3, 0.1), glm::vec3(0.4, 0.4, 0.3), 50.);
        Material rubber(glm::vec3(0.2, 0.8, 0.2), glm::vec3(0.1, 0.3, 0.1), 75.);
        Material mirror(glm::vec3(0.0, 10.0, 0.8), glm::vec3(1.0, 1.0, 1.0), 1425.);
        std::vector<Sphere> spheres;
        spheres.push_back(Sphere(glm::vec3(-3, 0, -16), 2, ivory));
        spheres.push_back(Sphere(glm::vec3(-1.0, -1.5, -12), 2, mirror));
        spheres.push_back(Sphere(glm::vec3(1.5, -0.5, -18), 3, rubber));
        spheres.push_back(Sphere(glm::vec3(7, 5, -18), 4, mirror));
        std::vector<Light> lights;
        lights.push_back(Light(glm::vec3(-20, 20, 20), 1.5));
        lights.push_back(Light(glm::vec3(30, 50, -25), 1.8));
        lights.push_back(Light(glm::vec3(30, 20, 30), 1.7));
        render(spheres, lights);
    }
};

class Refractions {
public:
    struct Light {
        Light(const glm::vec3& p, const float& i) : position(p), intensity(i) {}
        glm::vec3 position;
        float intensity;
    };

    struct Material {
        Material(const float& r, const glm::vec4& a, const glm::vec3& color, const float& spec)
            : refractive_index(r), albedo(a), diffuse_color(color), specular_exponent(spec) {}
        Material() : refractive_index(1), albedo(1, 0, 0, 0), diffuse_color(), specular_exponent() {}
        float refractive_index;
        glm::vec4 albedo;
        glm::vec3 diffuse_color;
        float specular_exponent;
    };

    struct Sphere {
        glm::vec3 center;
        float radius;
        Material material;
        Sphere(const glm::vec3& c, const float& r, const Material& m) : center(c), radius(r), material(m) {}
        bool ray_intersect(const glm::vec3& orig, const glm::vec3& dir, float& t0) const {
            glm::vec3 L = center - orig;
            float tca = glm::dot(L, dir);
            float d2 = glm::dot(L, L) - glm::dot(tca, tca);
            if (d2 > radius* radius) return false;
            float thc = sqrtf(radius * radius - d2);
            t0 = tca - thc;
            float t1 = tca + thc;
            if (t0 < 0) t0 = t1;
            if (t0 < 0) return false;
            return true;
        }
    };

    static void framebufferToJPG(const int width, const int height, std::vector<glm::vec3> fb) {
        std::vector<char> image(width * height * 3);
        for (size_t i = 0; i < height * width; ++i) {
            glm::vec3& c = fb[i];
            float max = std::max(c[0], std::max(c[1], c[2]));
            if (max > 1) c = c * (1.0f / max);
            image[i * 3] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].x * 255));
            image[(i * 3) + 1] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].y * 255));
            image[(i * 3) + 2] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].z * 255));
        }
        stbi_write_jpg("../assets/raytracing/08_Refractions.jpg", 1024, 768, 3, image.data(), 50);
    }

    static bool scene_intersect(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres, glm::
        vec3& hit, glm::vec3& N, Material& material) {
        float spheres_dist = std::numeric_limits<float>::max();
        for (size_t i = 0; i < spheres.size(); i++) {
            float dist_i;
            if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
                spheres_dist = dist_i;
                hit = orig + dir * dist_i;
                N = glm::normalize(hit - spheres[i].center);
                material = spheres[i].material;
            }
        }
        return spheres_dist < 1000;
    }

    static glm::vec3 refract(const glm::vec3& I, const glm::vec3& N, const float& refractive_index) { // Snell's law
        float cosi = -std::max(-1.f, std::min(1.f, glm::dot(I, N)));
        float etai = 1, etat = refractive_index;
        glm::vec3 n = N;
        if (cosi < 0) {
            // if the ray is inside the object, swap the indices and invert the normal to get the correct result
            cosi = -cosi;
            std::swap(etai, etat); n = -N;
        }
        float eta = etai / etat;
        float k = 1 - eta * eta * (1 - cosi * cosi);
        return k < 0 ? glm::vec3(0, 0, 0) : I * eta + n * (eta * cosi - sqrtf(k));
    }

    static glm::vec3 reflect(const glm::vec3& I, const glm::vec3& N) {
        return I - N * 2.f * glm::dot(I, N);
    }

    static glm::vec3 cast_ray(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres,
        const std::vector<Light>& lights, size_t depth = 0) {
        glm::vec3 point, N;
        Material material;
        if (depth > 4 || !scene_intersect(orig, dir, spheres, point, N, material)) {
            return glm::vec3(0.2, 0.2, 0.2); // background color
        }
        glm::vec3 reflect_dir = glm::normalize(reflect(dir, N));
        glm::vec3 refract_dir = glm::normalize(refract(dir, N, material.refractive_index));
        // offset the original point to avoid occlusion by the object itself
        glm::vec3 reflect_orig = glm::dot(reflect_dir, N) < 0 ? point - N * 0.001f : point + N * 0.001f;
        glm::vec3 refract_orig = glm::dot(refract_dir, N) < 0 ? point - N * 0.001f : point + N * 0.001f;
        glm::vec3 reflect_color = cast_ray(reflect_orig, reflect_dir, spheres, lights, depth + 1);
        glm::vec3 refract_color = cast_ray(refract_orig, refract_dir, spheres, lights, depth + 1);
        float diffuse_light_intensity = 0, specular_light_intensity = 0;
        for (size_t i = 0; i < lights.size(); i++) {
            glm::vec3 light_dir = glm::normalize(lights[i].position - point);
            float light_distance = glm::length(lights[i].position - point);
            // checking if the point lies in the shadow of the lights[i]
            glm::vec3 shadow_orig = glm::dot(light_dir, N) < 0 ? point - N * 0.001f : point + N * 0.001f;
            glm::vec3 shadow_pt, shadow_N;
            Material tmpmaterial;
            if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_N, tmpmaterial) &&
                glm::length(shadow_pt - shadow_orig) < light_distance)
                continue;
            diffuse_light_intensity += lights[i].intensity * std::max(0.f, glm::dot(light_dir, N));
            specular_light_intensity += powf(std::max(0.f, glm::dot(-reflect(-light_dir, N), dir)),
                material.specular_exponent) * lights[i].intensity;
        }
        return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + glm::vec3(1., 1., 1.) *
            specular_light_intensity * material.albedo[1] + reflect_color * material.albedo[2] + refract_color
            * material.albedo[3];
    }

    static void render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
        const int width = 1024;
        const int height = 768;
        const int fov = glm::pi<float>() / 2.;
        std::vector<glm::vec3> framebuffer(width * height);
        for (size_t j = 0; j < height; j++) {
            for (size_t i = 0; i < width; i++) {
                float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
                float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
                glm::vec3 dir = glm::normalize(glm::vec3(x, y, -1));
                framebuffer[i + j * width] = cast_ray(glm::vec3(0, 0, 0), dir, spheres, lights);
            }
        }
        framebufferToJPG(width, height, framebuffer);
    }

    static void main() {
        Material ivory(1.0, glm::vec4(0.6, 0.3, 0.1, 0.0), glm::vec3(0.4, 0.4, 0.3), 50.);
        Material glass(1.5, glm::vec4(0.0, 0.5, 0.1, 0.8), glm::vec3(0.6, 0.7, 0.8), 125.);
        Material rubber(1.0, glm::vec4(0.2, 0.8, 0.2, 0.0), glm::vec3(0.1, 0.3, 0.1), 75.);
        Material mirror(1.0, glm::vec4(0.0, 10.0, 0.8, 0.0), glm::vec3(1.0, 1.0, 1.0), 1425.);
        std::vector<Sphere> spheres;
        spheres.push_back(Sphere(glm::vec3(-3, 0, -16), 2, ivory));
        spheres.push_back(Sphere(glm::vec3(-1.0, -1.5, -12), 2, glass));
        spheres.push_back(Sphere(glm::vec3(1.5, -0.5, -18), 3, rubber));
        spheres.push_back(Sphere(glm::vec3(7, 5, -18), 4, mirror));
        std::vector<Light> lights;
        lights.push_back(Light(glm::vec3(-20, 20, 20), 1.5));
        lights.push_back(Light(glm::vec3(30, 50, -25), 1.8));
        lights.push_back(Light(glm::vec3(30, 20, 30), 1.7));
        render(spheres, lights);
    }
};

class Planes {
public:
    struct Light {
        Light(const glm::vec3& p, const float& i) : position(p), intensity(i) {}
        glm::vec3 position;
        float intensity;
    };

    struct Material {
        Material(const float& r, const glm::vec4& a, const glm::vec3& color, const float& spec)
            : refractive_index(r), albedo(a), diffuse_color(color), specular_exponent(spec) {}
        Material() : refractive_index(1), albedo(1, 0, 0, 0), diffuse_color(), specular_exponent() {}
        float refractive_index;
        glm::vec4 albedo;
        glm::vec3 diffuse_color;
        float specular_exponent;
    };

    struct Sphere {
        glm::vec3 center;
        float radius;
        Material material;
        Sphere(const glm::vec3& c, const float& r, const Material& m) : center(c), radius(r), material(m) {}
        bool ray_intersect(const glm::vec3& orig, const glm::vec3& dir, float& t0) const {
            glm::vec3 L = center - orig;
            float tca = glm::dot(L, dir);
            float d2 = glm::dot(L, L) - glm::dot(tca, tca);
            if (d2 > radius* radius) return false;
            float thc = sqrtf(radius * radius - d2);
            t0 = tca - thc;
            float t1 = tca + thc;
            if (t0 < 0) t0 = t1;
            if (t0 < 0) return false;
            return true;
        }
    };

    static void framebufferToJPG(const int width, const int height, std::vector<glm::vec3> fb) {
        std::vector<char> image(width * height * 3);
        for (size_t i = 0; i < height * width; ++i) {
            glm::vec3& c = fb[i];
            float max = std::max(c[0], std::max(c[1], c[2]));
            if (max > 1) c = c * (1.0f / max);
            image[i * 3] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].x * 255));
            image[(i * 3) + 1] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].y * 255));
            image[(i * 3) + 2] = std::min(static_cast<uint8_t>(255), static_cast<uint8_t>(fb[i].z * 255));
        }
        stbi_write_jpg("../assets/raytracing/09_Planes.jpg", 1024, 768, 3, image.data(), 50);
    }

    static bool scene_intersect(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres,
        glm::vec3& hit, glm::vec3& N, Material& material) {
        float spheres_dist = std::numeric_limits<float>::max();
        for (size_t i = 0; i < spheres.size(); i++) {
            float dist_i;
            if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
                spheres_dist = dist_i;
                hit = orig + dir * dist_i;
                N = glm::normalize(hit - spheres[i].center);
                material = spheres[i].material;
            }
        }
        float checkerboard_dist = std::numeric_limits<float>::max();
        if (fabs(dir.y) > 1e-3) {
            float d = -(orig.y + 4) / dir.y; // the checkerboard plane has equation y = -4
            glm::vec3 pt = orig + dir * d;
            if (d > 0 && fabs(pt.x) < 10 && pt.z<-10 && pt.z>-30 && d < spheres_dist) {
                checkerboard_dist = d;
                hit = pt;
                N = glm::vec3(0, 1, 0);
                material.diffuse_color = (int(.5 * hit.x + 1000) + int(.5 * hit.z)) & 1 ?
                    glm::vec3(1, 1, 1) : glm::vec3(.8, .2, .2);
                material.diffuse_color = material.diffuse_color * 0.3f;
            }
        }
        return std::min(spheres_dist, checkerboard_dist) < 1000;
    }

    static glm::vec3 refract(const glm::vec3& I, const glm::vec3& N, const float& refractive_index) { // Snell's law
        float cosi = -std::max(-1.f, std::min(1.f, glm::dot(I, N)));
        float etai = 1, etat = refractive_index;
        glm::vec3 n = N;
        if (cosi < 0) {
            // if the ray is inside the object, swap the indices and invert the normal to get the correct result
            cosi = -cosi;
            std::swap(etai, etat); n = -N;
        }
        float eta = etai / etat;
        float k = 1 - eta * eta * (1 - cosi * cosi);
        return k < 0 ? glm::vec3(0, 0, 0) : I * eta + n * (eta * cosi - sqrtf(k));
    }

    static glm::vec3 reflect(const glm::vec3& I, const glm::vec3& N) {
        return I - N * 2.f * glm::dot(I, N);
    }

    static glm::vec3 cast_ray(const glm::vec3& orig, const glm::vec3& dir, const std::vector<Sphere>& spheres,
        const std::vector<Light>& lights, size_t depth = 0) {
        glm::vec3 point, N;
        Material material;
        if (depth > 4 || !scene_intersect(orig, dir, spheres, point, N, material)) {
            return glm::vec3(0.2, 0.2, 0.2); // background color
        }
        glm::vec3 reflect_dir = glm::normalize(reflect(dir, N));
        glm::vec3 refract_dir = glm::normalize(refract(dir, N, material.refractive_index));
        // offset the original point to avoid occlusion by the object itself
        glm::vec3 reflect_orig = glm::dot(reflect_dir, N) < 0 ? point - N * 0.001f : point + N * 0.001f;
        glm::vec3 refract_orig = glm::dot(refract_dir, N) < 0 ? point - N * 0.001f : point + N * 0.001f;
        glm::vec3 reflect_color = cast_ray(reflect_orig, reflect_dir, spheres, lights, depth + 1);
        glm::vec3 refract_color = cast_ray(refract_orig, refract_dir, spheres, lights, depth + 1);
        float diffuse_light_intensity = 0, specular_light_intensity = 0;
        for (size_t i = 0; i < lights.size(); i++) {
            glm::vec3 light_dir = glm::normalize(lights[i].position - point);
            float light_distance = glm::length(lights[i].position - point);
            // checking if the point lies in the shadow of the lights[i]
            glm::vec3 shadow_orig = glm::dot(light_dir, N) < 0 ? point - N * 0.001f : point + N * 0.001f;
            glm::vec3 shadow_pt, shadow_N;
            Material tmpmaterial;
            if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_N, tmpmaterial) &&
                glm::length(shadow_pt - shadow_orig) < light_distance)
                continue;
            diffuse_light_intensity += lights[i].intensity * std::max(0.f, glm::dot(light_dir, N));
            specular_light_intensity += powf(std::max(0.f, glm::dot(-reflect(-light_dir, N), dir)),
                material.specular_exponent) * lights[i].intensity;
        }
        return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + glm::vec3(1., 1., 1.) *
            specular_light_intensity * material.albedo[1] + reflect_color * material.albedo[2] + refract_color
            * material.albedo[3];
    }

    static void render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
        const int width = 1024;
        const int height = 768;
        const int fov = glm::pi<float>() / 2.;
        std::vector<glm::vec3> framebuffer(width * height);
        for (size_t j = 0; j < height; j++) {
            for (size_t i = 0; i < width; i++) {
                float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
                float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
                glm::vec3 dir = glm::normalize(glm::vec3(x, y, -1));
                framebuffer[i + j * width] = cast_ray(glm::vec3(0, 0, 0), dir, spheres, lights);
            }
        }
        framebufferToJPG(width, height, framebuffer);
    }

    static void main() {
        Material ivory(1.0, glm::vec4(0.6, 0.3, 0.1, 0.0), glm::vec3(0.4, 0.4, 0.3), 50.);
        Material glass(1.5, glm::vec4(0.0, 0.5, 0.1, 0.8), glm::vec3(0.6, 0.7, 0.8), 125.);
        Material rubber(1.0, glm::vec4(0.2, 0.8, 0.2, 0.0), glm::vec3(0.1, 0.3, 0.1), 75.);
        Material mirror(1.0, glm::vec4(0.0, 10.0, 0.8, 0.0), glm::vec3(1.0, 1.0, 1.0), 1425.);
        std::vector<Sphere> spheres;
        spheres.push_back(Sphere(glm::vec3(-3, 0, -16), 2, ivory));
        spheres.push_back(Sphere(glm::vec3(-1.0, -1.5, -12), 2, glass));
        spheres.push_back(Sphere(glm::vec3(1.5, -0.5, -18), 3, rubber));
        spheres.push_back(Sphere(glm::vec3(7, 5, -18), 4, mirror));
        std::vector<Light> lights;
        lights.push_back(Light(glm::vec3(-20, 20, 20), 1.5));
        lights.push_back(Light(glm::vec3(30, 50, -25), 1.8));
        lights.push_back(Light(glm::vec3(30, 20, 30), 1.7));
        render(spheres, lights);
    }
};

void run(std::function<void()> f, const std::string& name) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> seconds = finish - start;
    std::cout << name << " Time = " << seconds.count() << std::endl;
}

int main() {
    run(SaveImage::main, "SaveImage");
    run(RayTracing::main, "RayTraciug");
    run(MoreSpheres::main, "MoreSpheres");
    run(Lighting::main, "Lighting");
    run(Specular::main, "Specular");
    run(Shadows::main, "Shadows");
    run(Reflections::main, "Reflections");
    run(Refractions::main, "Refractions");
    run(Planes::main, "Planes");

    return 0;
}