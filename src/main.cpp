#include <cfloat>

#include "material.h"
#include "util.h"

Vec3 background(const Ray& ray) {
  Vec3 n = normalize(ray.direction);
  Float t = (n.y + 1) * 0.5;
  return (1 - t) * Vec3(1) + t * Vec3(0.5, 0.7, 1.0);
}

class Hitable {
 public:
  virtual bool hit(const Ray& r, Float t_min, Float t_max,
                   HitRecord* record) const = 0;
};

class Sphere : public Hitable {
 public:
  Vec3 center;
  Float radius;
  const Material* material = nullptr;

  explicit Sphere(const Vec3& center, Float radius, const Material* material)
      : center(center), radius(radius), material(material) {}

  bool hit(const Ray& r, Float t_min, Float t_max,
           HitRecord* record) const override {
    Vec3 oc = r.origin - center;
    Float a = dot(r.direction, r.direction);
    Float b = dot(oc, r.direction);
    Float c = dot(oc, oc) - radius * radius;
    Float D = b * b - a * c;
    if (D > 0) {
      Float d = std::sqrt(D);
      Float t1 = (-b - d) / a;
      if (t1 < t_max && t1 > t_min) {
        record->t = t1;
        record->position = r.origin + r.direction * t1;
        record->normal = normalize(record->position - center);
        record->material = material;
        return true;
      }

      t1 = (-b + d) / a;
      if (t1 < t_max && t1 > t_min) {
        record->t = t1;
        record->position = r.origin + r.direction * t1;
        record->normal = normalize(record->position - center);
        record->material = material;
        return true;
      }
    }

    return false;
  }
};

class HitableList : public Hitable {
 public:
  explicit HitableList(Hitable** hit_list, int list_size) {
    list_ = {hit_list, hit_list + list_size};
  }

  bool hit(const Ray& r, Float t_min, Float t_max,
           HitRecord* record) const override {
    bool hit_anything = false;
    record->t = t_max;
    for (const auto& t : list_) {
      HitRecord temp;
      if (t->hit(r, t_min, t_max, &temp) && temp.t < record->t) {
        *record = temp;
        hit_anything = true;
      }
    }

    return hit_anything;
  }

  std::vector<Hitable*> list_;
};

class Camera {
 public:
  Camera() {
    lower_left_corner = Vec3(-2, 1, -1);
    horizontal = Vec3(4, 0, 0);
    vertical = Vec3(0, -2, 0);
    origin = Vec3(0);
  }

  Ray get_ray(Float u, Float v) {
    return Ray(origin,
               lower_left_corner + u * horizontal + v * vertical - origin);
  }

 private:
  Vec3 origin;
  Vec3 lower_left_corner;
  Vec3 horizontal;
  Vec3 vertical;
};

Vec3 color(const Ray& r, Hitable* world, int depth) {
  HitRecord rec;
  if (world->hit(r, 0.001, FLT_MAX, &rec)) {
    Ray scattered;
    Vec3 attenuation;
    if (depth < 50 && rec.material->scatter(r, rec, attenuation, scattered)) {
      return attenuation * color(scattered, world, depth + 1);
    } else {
      return Vec3(0);
    }
  }

  return background(r);
}

Vec3 gamma_correct(const Vec3& v) {
  return Vec3(std::sqrt(v.x), std::sqrt(v.y), std::sqrt(v.z));
}

int main(int argc, char* argv[]) {
  const int nx = 200;
  const int ny = 100;
  const int ns = 100;
  Bitmap bitmap(nx, ny);

  const Vec3 lower_left_corner(-2, 1, -1);
  const Vec3 horizontal(4, 0, 0);
  const Vec3 vertical(0, -2, 0);
  const Vec3 origin(0);

  Hitable* list[5];
  list[0] =
      new Sphere(Vec3(0, 0, -1), 0.5f, new Lambertian(Vec3(0.1, 0.2, 0.5)));
  list[1] =
      new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0)));
  list[2] =
      new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.0));
  list[3] = new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
  list[4] = new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5));

  Hitable* world = new HitableList(list, 5);

  Camera camera;
  for (int row = 0; row < bitmap.rows; row++) {
    for (int col = 0; col < bitmap.cols; ++col) {
      Vec3 val;
      for (int s = 0; s < ns; s++) {
        Float u = Float(col + drand48()) / Float(bitmap.cols);
        Float v = Float(row + drand48()) / Float(bitmap.rows);

        Ray ray = camera.get_ray(u, v);
        val += color(ray, world, 0);
      }
      val /= Float(ns);
      bitmap[row][col] = gamma_correct(val);
    }
  }

  save_as_png(bitmap, "board.png");
}