#pragma once

#include <array>
#include <cmath>
#include <vector>

#include "constants.hpp"
#include "models.hpp"

namespace RT::Rays {

using namespace RT::Models;
using namespace RT::Constants;

void generate_rays(std::array<Ray, RAYS_NUMBER>& rays, int x_start, int y_start);

void compute_ray(std::vector<unsigned int>& pixel_buffer,
                 const std::array<Ray, RAYS_NUMBER>& rays,
                 const std::vector<Circle>& obstacles,
                 size_t start,
                 size_t end);

void compute_rays(std::vector<unsigned int>& pixel_buffer,
                  const std::array<Ray, RAYS_NUMBER>& rays,
                  const std::vector<Circle>& obstacles,
                  int num_threads);
}  // namespace RT::Rays
