#include "boid.h"
#include "raylib.h"
#include "raymath.h"

Boid::Boid() : pos(Vector2Zero()), vel(Vector2Zero()), radius(1.f) {}
Boid::Boid(const Vector2 pos_i, const Vector2 vel_i, const float radius_i) : pos(pos_i), vel(vel_i), radius(radius_i) {}


void Boid::update(const float max_vel) {
  const int width = GetScreenWidth();
  const int height = GetScreenHeight();

  this->pos.x += this->vel.x;
  this->pos.y += this->vel.y;

  // Wrap around window.
  if (this->pos.x > width) {
    this->pos.x = 0.f;
  } else if (this->pos.x < 0.f) {
    this->pos.x = width;
  }

  if (this->pos.y > height) {
    this->pos.y = 0.f;
  } else if (this->pos.y < 0.f) {
    this->pos.y = height;
  }

  // Limit velocity
  if (this->vel.x > max_vel) { this->vel.x = max_vel; }
  else if (this->vel.x < -max_vel) { this->vel.x = -max_vel; }

  if (this->vel.y > max_vel) { this->vel.y = max_vel; }
  else if (this->vel.y < -max_vel) { this->vel.y = -max_vel; }
}

void Boid::draw(bool draw_outlines, const float avoid_radius, const float align_radius) {
  DrawCircleV(this->pos, this->radius, WHITE);

  if (draw_outlines) {
    // Velocity vector
    {
      Vector2 v = Vector2Scale(this->vel, 4.f);
      Vector2 direction = Vector2Add(this->pos, v);
      DrawLineEx(this->pos, direction, 2.f, GREEN);
    }

    // Seperation radius
    DrawCircleLinesV(this->pos, avoid_radius, RED);

    // Visible radius
    DrawCircleLinesV(this->pos, align_radius, BLUE);
  }
}

void Boid::apply_separation(const std::vector<Boid> &boids, const float avoid_radius, const float avoid_factor) {
  Vector2 avoid_dv = Vector2Zero();
  for (const Boid &other_body : boids) {
    if (&other_body == this) {
      continue;
    }

    const float dist = Vector2Distance(this->pos, other_body.pos);
    if (dist > avoid_radius) {
      continue;
    }

    Vector2 close_dv = Vector2Subtract(this->pos, other_body.pos);
    avoid_dv = Vector2Add(avoid_dv, close_dv);
  }

  this->vel.x += avoid_dv.x * avoid_factor;
  this->vel.y += avoid_dv.y * avoid_factor;
}

void Boid::apply_alignment(const std::vector<Boid> &boids, const float align_radius, const float align_factor) {
  /*
   * Accumulate all the boids within the visible range's average velocity.
   */
  size_t close_bodies_count = 0;
  Vector2 avg_v = Vector2Zero();

  for (const Boid &other_body : boids) {
    if (&other_body == this) {
      continue;
    }

    const float dist = Vector2Distance(this->pos, other_body.pos);
    if (dist > align_radius) {
      continue;
    }

    close_bodies_count++;
    avg_v.x += other_body.vel.x;
    avg_v.y += other_body.vel.y;
  }

  if (close_bodies_count == 0) {
    return;
  }
  avg_v.x = avg_v.x / close_bodies_count;
  avg_v.y = avg_v.y / close_bodies_count;

  this->vel.x += (avg_v.x - this->vel.x) * align_factor;
  this->vel.y += (avg_v.y - this->vel.y) * align_factor;
}

void Boid::apply_cohesion(const std::vector<Boid> &boids, const float cohesion_radius, const float cohesion_factor) {
  /*
   * Obtain the center of mass of the flock.
   */
  Vector2 avg_pos = Vector2Zero();
  size_t close_bodies_count = 0;

  for (const Boid &other_body : boids) {
    if (&other_body == this) {
      continue;
    }

    const float dist = Vector2Distance(this->pos, other_body.pos);
    if (dist > cohesion_radius) {
      continue;
    }

    close_bodies_count++;
    avg_pos.x += other_body.pos.x;
    avg_pos.y += other_body.pos.y;
  }

  if (close_bodies_count == 0) {
    return;
  }
  avg_pos.x = avg_pos.x / close_bodies_count;
  avg_pos.y = avg_pos.y / close_bodies_count;

  this->vel.x += (avg_pos.x - this->pos.x) * cohesion_factor;
  this->vel.y += (avg_pos.y - this->pos.y) * cohesion_factor;
}
