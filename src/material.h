#ifndef MATERIAL_H
#define MATERIAL_H

#include "util.h"

class Material;
struct HitRecord {
  Float t;
  Vec3 position;
  Vec3 normal;
  const Material* material = nullptr;
};

class Material {
 public:
  virtual bool scatter(const Ray& ray_in, const HitRecord& hit_rec,
                       Vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
 public:
  Lambertian(const Vec3& albedo) : albedo(albedo) {}

  bool scatter(const Ray& ray_in, const HitRecord& hit_rec, Vec3& attenuation,
               Ray& scattered) const override {
    Vec3 target = hit_rec.position + hit_rec.normal + random_in_unit_sphere();
    scattered = Ray(hit_rec.position, target - hit_rec.position);
    attenuation = albedo;
    return true;
  }

 private:
  Vec3 albedo;
};

class Metal : public Material {
 public:
  Metal(Vec3 albedo, Float f) : albedo(albedo) { fuzz = f < 1 ? f : 1; }

  bool scatter(const Ray& ray_in, const HitRecord& hit_rec, Vec3& attenuation,
               Ray& scattered) const override {
    Vec3 reflected = reflect(normalize(ray_in.direction), hit_rec.normal);
    scattered =
        Ray(hit_rec.position, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    return dot(scattered.direction, hit_rec.normal) > 0;
  }

 private:
  Vec3 albedo;
  Float fuzz;
};

class Dielectric : public Material {
 public:
  Dielectric(Float ri) : ref_idx(ri) {}

  bool scatter(const Ray& ray_in, const HitRecord& hit_rec, Vec3& attenuation,
               Ray& scattered) const override {
    Vec3 outward_normal;
    Vec3 reflected = reflect(ray_in.direction, hit_rec.normal);

    Float ni_over_nt;
    attenuation = Vec3(1.0);
    Vec3 refracted;
    Float reflect_prob;
    Float cosine;

    if (dot(ray_in.direction, hit_rec.normal) > 0) {
      outward_normal = -hit_rec.normal;
      ni_over_nt = ref_idx;
      cosine = ref_idx * dot(ray_in.direction, hit_rec.normal) /
               length(ray_in.direction);
    } else {
      outward_normal = hit_rec.normal;
      ni_over_nt = 1.0 / ref_idx;
      cosine =
          -dot(ray_in.direction, hit_rec.normal) / length(ray_in.direction);
    }

    if (refract(ray_in.direction, outward_normal, ni_over_nt, refracted)) {
      reflect_prob = schlick(cosine, ref_idx);
    } else {
      scattered = Ray(hit_rec.position, reflected);
      reflect_prob = 1.0;
    }

    if (drand48() < reflect_prob) {
      scattered = Ray(hit_rec.position, reflected);
    } else {
      scattered = Ray(hit_rec.position, refracted);
    }

    return true;
  }

  Float ref_idx;
};

#endif
