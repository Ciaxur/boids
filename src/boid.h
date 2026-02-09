#pragma once
#include "raylib.h"
#include <vector>

class Boid {
private:
  Vector2 pos;
  Vector2 vel;
  float radius;

public:
  Boid();
  Boid(const Vector2 pos_i, const Vector2 vel_i, const float radius_i);
  ~Boid() = default;

  void update(const float max_vel);
  void draw(bool draw_outlines, const float avoid_radius, const float align_radius);

  void apply_separation(const std::vector<Boid> &boids, const float avoid_radius, const float avoid_factor);
  void apply_alignment(const std::vector<Boid> &boids, const float align_radius, const float align_factor);
  void apply_cohesion(const std::vector<Boid> &boids, const float cohesion_radius, const float cohesion_factor);
};
