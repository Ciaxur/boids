#include "boid.h"
#include <cstdio>
#include <cstdlib>
#include <fmt/base.h>
#include <fmt/format.h>
#include <random>
#include <raylib.h>
#include <vector>

#define WIDTH  1600
#define HEIGHT 1200

#define BOIDS_POPULATION 300
#define MAX_VELOCITY     3.f

#define BOIDS_RADIUS         2.f
#define BOIDS_AVOID_RADIUS   (BOIDS_RADIUS * 6.f)
#define BOIDS_VISIBLE_RADIUS (BOIDS_AVOID_RADIUS * 3)

#define BOIDS_AVOID_FACTOR    0.01f
#define BOIDS_ALIGN_FACTOR    0.1f
#define BOIDS_COHESION_FACTOR 0.001f


class UniformRandom {
private:
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<> rand;

public:
  UniformRandom() : rd(), gen(rd()), rand() {}
  ~UniformRandom() = default;
  float generate() { return this->rand(this->gen); }
};


int main() {
  UniformRandom r;

  InitWindow(WIDTH, HEIGHT, "Boids Algorithm");
  SetTargetFPS(60);

  std::vector<Boid> boids;
  boids.reserve(BOIDS_POPULATION * 1.5);
  for (size_t i = 0; i < BOIDS_POPULATION; i++) {
    Vector2 v{
      /*
       * Generate a value between -2.f and 2.f.
       *
       * Un-gated maths:
       *   1. r.generate() returns a random value between 0.0 and 1.0.
       *   2. shifting by -0.5, results in a random value between -0.5 and 0.5
       *   3. then multiplying by 2.0 scales range to -1.0 and 1.0
       *   4. then multiplying by 2.0 again scales range to -2.0 and 2.0
       */
      .x = ( ( r.generate() - 0.5f ) * 2.f ) * 2.f,
      .y = ( ( r.generate() - 0.5f ) * 2.f ) * 2.f,
    };

    Vector2 pos {
      .x = r.generate() * WIDTH,
      .y = r.generate() * HEIGHT,
    };

    Boid &_ = boids.emplace_back(Boid(pos, v, BOIDS_RADIUS));
  }

  bool draw_outlines = false;

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_Q)) {
      CloseWindow();
      break;
    }
    if (IsKeyPressed(KEY_SPACE)) {
      draw_outlines = !draw_outlines;
    }

    // Update
    for (Boid &boid : boids) {
      boid.apply_separation(boids, BOIDS_AVOID_RADIUS, BOIDS_AVOID_FACTOR);
      boid.apply_alignment(boids, BOIDS_VISIBLE_RADIUS, BOIDS_ALIGN_FACTOR);
      boid.apply_cohesion(boids, BOIDS_VISIBLE_RADIUS, BOIDS_COHESION_FACTOR);

      boid.update(MAX_VELOCITY);
    }

    BeginDrawing();
    ClearBackground(BLACK);

    for (Boid &boid : boids) {
      boid.draw(draw_outlines, BOIDS_AVOID_RADIUS, BOIDS_VISIBLE_RADIUS);
    }

    DrawFPS(WIDTH - 100.f, 10.f);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
