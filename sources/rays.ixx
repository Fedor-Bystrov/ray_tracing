export module rays;

import <array>;
import <numbers>;
import <vector>;
import <thread>;

import models;
import constants;

namespace RT::Rays {

using namespace RT::Models;
using namespace RT::Constants;

export void generate_rays(std::array<Ray, RAYS_NUMBER>& rays,
                          int x_start,
                          int y_start) {
  for (int i = 0; i < RAYS_NUMBER; i++) {
    double angle =
        (static_cast<double>(i) / RAYS_NUMBER) * 2 * std::numbers::pi;

    rays[i] = RT::Models::Ray{
        .x_start{x_start},
        .y_start{y_start},
        .angle{angle},
        .dx{std::cos(angle)},
        .dy{std::sin(angle)},
    };
  }
}

export void compute_ray(std::vector<uint32_t>& pixel_buffer,
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
      pixel_buffer[px + py * SCREEN_WIDTH] = COLOR_RAY;

      double dx = x - obstacle.x;
      double dy = y - obstacle.y;
      double dist_squared = dx * dx + dy * dy;

      if (dist_squared < obstacle_r2) {
        pixel_buffer[px + py * SCREEN_WIDTH] = COLOR_OBSTACLE;

        break;
      }

      x += ray.dx;
      y += ray.dy;
    }
  }
}

export void compute_rays(std::vector<uint32_t>& pixel_buffer,
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
}  // namespace RT::Rays