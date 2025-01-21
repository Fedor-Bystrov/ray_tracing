#include <SDL2/SDL.h>

import <print>;
import <array>;
import <cmath>;
import <numbers>;

constexpr int SCREEN_WIDTH{900};
constexpr int SCREEN_HEIGHT{600};

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

static void draw_circle(SDL_Renderer* renderer, Circle circle) {
  int r_squared = std::pow(circle.r, 2);
  for (int x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
    for (int y = circle.y - circle.r; y <= circle.y + circle.r; y++) {
      int dist_squared = std::pow(x - circle.x, 2) + std::pow(y - circle.y, 2);
      if (dist_squared < r_squared) {
        auto pixel = SDL_Rect{x, y, 1, 1};
        SDL_RenderFillRect(renderer, &pixel);
      }
    }
  }
}

static void draw_rays(SDL_Renderer* renderer,
                      const std::array<Ray, RAYS_NUMBER>& rays) {
  for (const auto& ray : rays) {
    float x{static_cast<float>(ray.x_start)};
    float y{static_cast<float>(ray.y_start)};

    bool end_of_screen{false};
    while (!end_of_screen) {
      x += std::cos(ray.angle);
      y += std::sin(ray.angle);

      auto pixel = SDL_FRect{x, y, 1, 1};
      SDL_RenderFillRectF(renderer, &pixel);

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
  Circle light_c{200, 200, 80};
  Circle shadow_c{650, 300, 140};

  generate_rays(rays, light_c.x, light_c.y);

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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_circle(renderer, light_c);
    draw_circle(renderer, shadow_c);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    draw_rays(renderer, rays);

    SDL_RenderPresent(renderer);

    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
