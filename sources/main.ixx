#include <SDL.h>

import <iostream>;
import <algorithm>;
import <array>;
import <cmath>;
import <numbers>;

constexpr int SCREEN_WIDTH{900};
constexpr int SCREEN_HEIGHT{600};

constexpr int COLOR_WHITE{0xffffff};
constexpr int COLOR_BLACK{0x000000};
constexpr int COLOR_GREY{0xefefef};

constexpr int RAYS_NUMBER{100};

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

static void draw_circle(SDL_Surface* surface, Circle circle, int color) {
  int r_squared = std::pow(circle.r, 2);
  for (int x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
    for (int y = circle.y - circle.r; y <= circle.y + circle.r; y++) {
      int dist_squared = std::pow(x - circle.x, 2) + std::pow(y - circle.y, 2);
      if (dist_squared < r_squared) {
        auto pixel = SDL_Rect{x, y, 1, 1};
        SDL_FillRect(surface, &pixel, color);
      }
    }
  }
}

static void draw_rays(SDL_Surface* surface,
                      const std::array<Ray, RAYS_NUMBER>& rays,
                      int color) {
  for (const auto& ray : rays) {
    int x{ray.x_start};
    int y{ray.y_start};

    bool end_of_screen{false};
    for (int i = 1; !end_of_screen; i++) {
      if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        end_of_screen = true;
      }

      x += i * std::cos(ray.angle);
      y += i * std::sin(ray.angle);

      auto pixel = SDL_Rect{x, y, 1, 1};
      SDL_FillRect(surface, &pixel, color);
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

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::printf("SDL could not be initialized! SDL_Error: %s\n",
                SDL_GetError());
    return 0;
  }

  // Create window
  auto* window =
      SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

  auto* surface = SDL_GetWindowSurface(window);
  auto erase_rect = SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

  std::array<Ray, RAYS_NUMBER> rays{};
  Circle light_c{200, 200, 80};
  Circle shadow_c{650, 300, 140};

  bool quit{false};
  SDL_Event e;

  while (!quit) {
    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT) {
      quit = true;
    }

    if (e.type == SDL_MOUSEMOTION && e.motion.state != 0) {
      light_c.x = e.motion.x;
      light_c.y = e.motion.y;
      generate_rays(rays, light_c.x, light_c.y);
    }

    // Reset screen
    SDL_FillRect(surface, &erase_rect, COLOR_BLACK);

    draw_circle(surface, light_c, COLOR_WHITE);
    draw_circle(surface, shadow_c, COLOR_WHITE);
    draw_rays(surface, rays, COLOR_GREY);

    SDL_UpdateWindowSurface(window);

    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
