#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <cstdint>
#include <vector>

using Float = float;

struct Vec3 {
  Float x, y, z;
  Vec3() : x(0), y(0), z(0) {}
  explicit Vec3(Float a) : x(a), y(a), z(a) {}
  explicit Vec3(Float x_, Float y_, Float z_) : x(x_), y(y_), z(z_) {}

  Vec3 operator-() const { return Vec3(-x, -y, -z); }
  Float& operator[](int i) { return (&x)[i]; }
  const Float& operator[](int i) const { return (&x)[i]; }

  Vec3 operator+(const Vec3& rhs) const {
    return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
  }
  Vec3 operator+(Float a) const { return Vec3(x + a, y + a, z + a); }
  Vec3 operator-(const Vec3& rhs) const {
    return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
  }
  Vec3 operator*(const Vec3& rhs) const {
    return Vec3(x * rhs.x, y * rhs.y, z * rhs.z);
  }
  Vec3 operator*(Float a) const { return Vec3(x * a, y * a, z * a); }
  Vec3 operator/(const Vec3& rhs) const {
    return Vec3(x / rhs.x, y / rhs.y, z / rhs.z);
  }

  Vec3& operator+=(const Vec3& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  Vec3& operator-=(const Vec3& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  Vec3& operator*=(const Vec3& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
  }
  Vec3& operator/=(const Vec3& rhs) {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
  }
};

inline Vec3 operator*(Float t, const Vec3& v) { return v * t; }

inline Float dot(const Vec3& v1, const Vec3& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Float squared_length(const Vec3& v) { return dot(v, v); }

inline Float length(const Vec3& v) { return std::sqrt(squared_length(v)); }

inline Vec3 normalize(const Vec3& v) {
  Float len = length(v);
  return v * (1 / len);
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
  Float x = v1.y * v2.z - v1.z * v2.y;
  Float y = v1.z * v2.x - v1.x * v2.z;
  Float z = v1.x * v2.y - v1.y * v2.x;
  return Vec3(x, y, z);
}

struct Bitmap {
  explicit Bitmap(int cols, int rows) : cols(cols), rows(rows) {
    const int total = cols * rows;
    data.resize(total);
  }

  Vec3* operator[](int row) {
    const int pos = row * cols;
    return &data[pos];
  }

  const Vec3* operator[](int row) const {
    const int pos = row * cols;
    return &data[pos];
  }

  const int cols, rows;
  std::vector<Vec3> data;
};

struct Ray {
  Vec3 origin;
  Vec3 direction;

  Ray() = default;
  explicit Ray(const Vec3& origin, const Vec3& direction)
      : origin(origin), direction(direction) {}
};

// example: save_as_png(bitmap, "output.png");
void save_as_png(const Bitmap& bitmap, const char* filename);

#endif