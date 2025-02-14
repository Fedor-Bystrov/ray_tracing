#pragma once

#include <array>
#include <vector>
#include <thread>
#include <cmath>

namespace RT {

    namespace Models {
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

    } // namespace Models


    namespace Constants {

        constexpr int SCREEN_WIDTH{1200};
        constexpr int SCREEN_HEIGHT{900};
        constexpr int PIXELS{SCREEN_WIDTH * SCREEN_HEIGHT};
        
        constexpr unsigned int COLOR_BLACK{0x00000000};
        constexpr unsigned int COLOR_RAY{0xFFFFD700};
        constexpr unsigned int COLOR_OBSTACLE{0xFFDC143C};
        
        constexpr int RAYS_NUMBER{1000};
        
    } // namespace Constants

    namespace Rays {

        using namespace RT::Models;
        using namespace RT::Constants;
    
        void generate_rays(std::array<Ray, RAYS_NUMBER>& rays,
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
    
        void compute_ray(std::vector<uint32_t>& pixel_buffer,
            const std::array<Ray, RAYS_NUMBER>& rays,
            const std::vector<Circle>& obstacles,
            size_t start,
            size_t end) {
            auto obstacle0_r2 = static_cast<double>(obstacles[0].r) * obstacles[0].r;
            auto obstacle1_r2 = static_cast<double>(obstacles[1].r) * obstacles[1].r;
    
            for (size_t i = start; i < end; ++i) {
                const auto& ray = rays[i];
                double x = static_cast<double>(ray.x_start);
                double y = static_cast<double>(ray.y_start);
    
                while (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
                    int px = static_cast<int>(x);
                    int py = static_cast<int>(y);
                    pixel_buffer[px + py * SCREEN_WIDTH] = COLOR_RAY;
    
                    double dx0 = x - obstacles[0].x;
                    double dy0 = y - obstacles[0].y;
                    double dist_squared0 = dx0 * dx0 + dy0 * dy0;
    
                    double dx1 = x - obstacles[1].x;
                    double dy1 = y - obstacles[1].y;
                    double dist_squared1 = dx1 * dx1 + dy1 * dy1;
    
                    if (dist_squared0 < obstacle0_r2 || dist_squared1 < obstacle1_r2) {
                        pixel_buffer[px + py * SCREEN_WIDTH] = COLOR_OBSTACLE;
                        break;
                    }
    
                    x += ray.dx;
                    y += ray.dy;
                }
            }
        }
    
        void compute_rays(std::vector<uint32_t>& pixel_buffer,
            const std::array<Ray, RAYS_NUMBER>& rays,
            const std::vector<Circle>& obstacles,
            int num_threads) {
            std::vector<std::thread> threads;
            size_t rays_per_thread = RAYS_NUMBER / num_threads;
    
            for (size_t t = 0; t < num_threads; ++t) {
                size_t start = t * rays_per_thread;
                size_t end = (t == num_threads - 1) ? RAYS_NUMBER : start + rays_per_thread;
                threads.emplace_back(
                    [&, start, end]() {
                        compute_ray(pixel_buffer, rays, obstacles, start, end);
                    });
            }
    
            for (auto& thread : threads) {
                thread.join();
            }
        }
    }  // namespace Rays

}  // namespace RT
