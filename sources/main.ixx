#include <SDL2/SDL.h>

import <print>;
import <array>;
import <vector>;

import constants;
import models;
import rays;

using namespace RT::Models;
using namespace RT::Constants;
using namespace RT::Rays;

static void draw_rays(SDL_Renderer* r,
                      const std::array<Ray, RAYS_NUMBER>& rays,
                      const Circle& obstacle) {
  std::vector<uint32_t> pixel_buffer(PIXELS, COLOR_BLACK);

  compute_rays(pixel_buffer, rays, obstacle, 4);

  auto texture =
      SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
                        SCREEN_WIDTH, SCREEN_HEIGHT);

  SDL_UpdateTexture(texture, nullptr, pixel_buffer.data(),
                    SCREEN_WIDTH * sizeof(uint32_t));

  SDL_RenderCopy(r, texture, nullptr, nullptr);
  SDL_DestroyTexture(texture);
}

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

// TODO: Add multiple obstacles, try processing in parallel
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
