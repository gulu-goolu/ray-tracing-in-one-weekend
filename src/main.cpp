#include "util.h"

Vec3 background(const Ray& ray) {
  Vec3 n = normalize(ray.direction);
  Float t = (n.y + 1) * 0.5;
  return (1 - t) * Vec3(1) + t * Vec3(0.5, 0.7, 1.0);
}

class Sphere {
 public:
  Vec3 center;
  Float radius;

  bool hit(const Ray& ray) const {
    Vec3 oc = ray.origin - center;
    Float a = dot(ray.direction, ray.direction);
    Float b = 2 * dot(oc, ray.direction);
    Float c = dot(oc, oc) - radius * radius;
    Float D = b * b - 4 * a * c;
    return D > 0;
  }
};

Vec3 color(const Ray& r) {
  Sphere spehere{Vec3(0, 0, -1), 0.5};
  if (spehere.hit(r)) {
    return Vec3(1, 0, 0);
  }

  return background(r);
}

int main(int argc, char* argv[]) {
  const int nx = 200;
  const int ny = 100;
  Bitmap bitmap(nx, ny);

  const Vec3 lower_left_corner(-2, 1, -1);
  const Vec3 horizontal(4, 0, 0);
  const Vec3 vertical(0, -2, 0);
  const Vec3 origin(0);
  for (int row = 0; row < bitmap.rows; row++) {
    for (int col = 0; col < bitmap.cols; ++col) {
      Float u = Float(col) / Float(bitmap.cols);
      Float v = Float(row) / Float(bitmap.rows);

      Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
      bitmap[row][col] = color(ray);
    }
  }

  save_as_png(bitmap, "board.png");
}