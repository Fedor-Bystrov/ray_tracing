#include <SDL2/SDL.h>

import <print>;
import <array>;
import <cmath>;
import <numbers>;
import <vector>;
import <thread>;

constexpr int SCREEN_WIDTH{1200};
constexpr int SCREEN_HEIGHT{900};
constexpr int PIXELS{SCREEN_WIDTH * SCREEN_HEIGHT};

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
  double dx;  // Precomputed cosine
  double dy;  // Precomputed sine
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

static void generate_rays(std::array<Ray, RAYS_NUMBER>& rays,
                          int x_start,
                          int y_start) {
  for (int i = 0; i < RAYS_NUMBER; i++) {
    double angle =
        (static_cast<double>(i) / RAYS_NUMBER) * 2 * std::numbers::pi;

    rays[i] = Ray{
        .x_start{x_start},
        .y_start{y_start},
        .angle{angle},
        .dx{std::cos(angle)},
        .dy{std::sin(angle)},
    };
  }
}

static void compute_ray(std::vector<uint32_t>& pixel_buffer,
                        const std::array<Ray, RAYS_NUMBER>& rays,
                        const Circle& obstacle,
                        size_t start,
                        size_t end) {
  auto obstacle_r2 = static_cast<double>(obstacle.r) * obstacle.r;

  for (size_t i = start; i < end; ++i) {
    const auto& ray = rays[i];
    double x = static_cast<double>(ray.x_start);
    double y = static_cast<double>(ray.y_start);

    while (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
      int px = static_cast<int>(x);
      int py = static_cast<int>(y);
      pixel_buffer[px + py * SCREEN_WIDTH] = 0xFFFF00FF;

      double dx = x - obstacle.x;
      double dy = y - obstacle.y;
      double dist_squared = dx * dx + dy * dy;

      if (dist_squared < obstacle_r2) {
        break;
      }

      x += ray.dx;
      y += ray.dy;
    }
  }
}

static void compute_rays(std::vector<uint32_t>& pixel_buffer,
                         const std::array<Ray, RAYS_NUMBER>& rays,
                         const Circle& obstacle,
                         int num_threads) {
  std::vector<std::thread> threads;
  size_t rays_per_thread = RAYS_NUMBER / num_threads;

  for (size_t t = 0; t < num_threads; ++t) {
    size_t start = t * rays_per_thread;
    size_t end = (t == num_threads - 1) ? RAYS_NUMBER : start + rays_per_thread;
    threads.emplace_back(
        [&](size_t start, size_t end) {
          compute_ray(pixel_buffer, rays, obstacle, start, end);
        },
        start, end);
  }

  for (auto& thread : threads) {
    thread.join();
  }
}

static void draw_rays(SDL_Renderer* r,
                      const std::array<Ray, RAYS_NUMBER>& rays,
                      const Circle& obstacle) {
  std::vector<uint32_t> pixel_buffer(PIXELS, 0x00000000);

  compute_rays(pixel_buffer, rays, obstacle, 4);

  auto texture =
      SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
                        SCREEN_WIDTH, SCREEN_HEIGHT);

  SDL_UpdateTexture(texture, nullptr, pixel_buffer.data(),
                    SCREEN_WIDTH * sizeof(uint32_t));

  SDL_RenderCopy(r, texture, nullptr, nullptr);
  SDL_DestroyTexture(texture);
}

// TODO: Add multiple obstacles, try processing in parallel
// TODO: Make colors and picture prettier
// TODO: Add description and demo to the Github repo

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::println("SDL could not be initialized! SDL_Error: {}", SDL_GetError());
    return 0;
  }

  auto* window =
      SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

  auto* renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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

    SDL_RenderClear(renderer);

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
