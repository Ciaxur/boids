#include <fmt/base.h>
#include <raylib.h>
#include "raymath.h"
#include <fmt/format.h>

#define WIDTH 800
#define HEIGHT 800

int main() {
  InitWindow(WIDTH, HEIGHT, "Window");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_Q)) {
      CloseWindow();
      break;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawFPS(WIDTH - 100.f, 10.f);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
