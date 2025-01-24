#include <SDL2/SDL.h>

import <print>;
import <array>;
import <cmath>;
import <numbers>;

constexpr int SCREEN_WIDTH{1200};
constexpr int SCREEN_HEIGHT{900};

constexpr int RAYS_NUMBER{500};

// TODO: add utility methods to structs
struct Circle {
  int x;
  int y;
  int r;
};

struct Ray {
  int x_start;
  int y_start;
  double angle;
};

// Bresenham's Algorithm
static void draw_circle(SDL_Renderer* r, const Circle& circle) {
  int x = circle.r;
  int y = 0;
  int direction = 0;

  while (x >= y) {
    // Draw the eight symmetric points of the circle
    SDL_RenderDrawPoint(r, circle.x + x, circle.y + y);
    SDL_RenderDrawPoint(r, circle.x + y, circle.y + x);
    SDL_RenderDrawPoint(r, circle.x - y, circle.y + x);
    SDL_RenderDrawPoint(r, circle.x - x, circle.y + y);
    SDL_RenderDrawPoint(r, circle.x - x, circle.y - y);
    SDL_RenderDrawPoint(r, circle.x - y, circle.y - x);
    SDL_RenderDrawPoint(r, circle.x + y, circle.y - x);
    SDL_RenderDrawPoint(r, circle.x + x, circle.y - y);

    y += 1;
    if (direction <= 0) {
      direction += 2 * y + 1;
    }
    if (direction > 0) {
      x -= 1;
      direction -= 2 * x + 1;
    }
  }
}

static void draw_rays(SDL_Renderer* renderer,
                      const std::array<Ray, RAYS_NUMBER>& rays,
                      const Circle& obs) {
  auto obs_r_squared = std::pow(obs.r, 2);
  for (const auto& ray : rays) {
    float x{static_cast<float>(ray.x_start)};
    float y{static_cast<float>(ray.y_start)};

    bool end_of_screen{false};
    while (!end_of_screen) {
      x += std::cos(ray.angle);
      y += std::sin(ray.angle);

      auto pixel = SDL_FRect{x, y, 1, 1};
      SDL_RenderFillRectF(renderer, &pixel);

      auto dist_squared = std::pow(x - obs.x, 2) + std::pow(y - obs.y, 2);
      if (dist_squared < obs_r_squared) {
        // Breaking since ray touched the obstacle
        break;
      }

      if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
        end_of_screen = true;
      }
    }
  }
}

static void generate_rays(std::array<Ray, RAYS_NUMBER>& rays,
                          int x_start,
                          int y_start) {
  for (int i = 0; i < RAYS_NUMBER; i++) {
    rays[i] = Ray{
        .x_start{x_start},
        .y_start{y_start},
        .angle{(static_cast<double>(i) / RAYS_NUMBER) * 2 * std::numbers::pi}};
  }
}

// TODO: 1. Clean up
// TODO: 2. Add multiple obstacles, try processing in parallel
// TODO: 3. Make colors and picture prettier
// TODO: 4. Add description and demo to the Github repo

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::println("SDL could not be initialized! SDL_Error: {}", SDL_GetError());
    return 0;
  }

  auto* window =
      SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

  auto* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  std::array<Ray, RAYS_NUMBER> rays{};
  Circle sun{200, 200, 40};
  Circle obstacle{820, 300, 140};

  generate_rays(rays, sun.x, sun.y);

  bool quit{false};
  SDL_Event e;

  int speed_y{10};

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }

      if (e.type == SDL_MOUSEMOTION && e.motion.state != 0) {
        sun.x = e.motion.x;
        sun.y = e.motion.y;
        generate_rays(rays, sun.x, sun.y);
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    draw_rays(renderer, rays, obstacle);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_circle(renderer, sun);
    draw_circle(renderer, obstacle);

    obstacle.y += speed_y;
    if (obstacle.y - obstacle.r <= 0 ||
        obstacle.y + obstacle.r > SCREEN_HEIGHT) {
      speed_y = -speed_y;
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
