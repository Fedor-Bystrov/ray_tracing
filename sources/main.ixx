#include <SDL.h>

import <iostream>;
import <algorithm>;
import <cmath>;

constexpr int SCREEN_WIDTH{ 900 };
constexpr int SCREEN_HEIGHT{ 600 };

constexpr int COLOR_WHITE{ 0xffffff };
constexpr int COLOR_BLACK{ 0x000000 };

struct Circle {
	int x;
	int y;
	int r;
};

void fill_circle(SDL_Surface* surface, Circle circle, int color) {
	int r_squared = std::pow(circle.r, 2);
	for (int x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
		for (int y = circle.y - circle.r; y <= circle.y + circle.r; y++) {
			int dist_squared = std::pow(x - circle.x, 2) + std::pow(y - circle.y, 2);
			if (dist_squared < r_squared) {
				auto pixel = SDL_Rect{ x, y, 1, 1 };
				SDL_FillRect(surface, &pixel, color);
			}
		}
	}
}

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::printf("SDL could not be initialized! SDL_Error: %s\n",
			SDL_GetError());
		return 0;
	}

	// Create window
	auto* window = SDL_CreateWindow(
		"Ray Tracing",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		0);

	auto* surface = SDL_GetWindowSurface(window);
	auto erase_rect = SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	Circle light_c{ 200, 200, 80 };
	Circle shadow_c{ 650, 300, 140 };

	bool quit{ false };
	SDL_Event e;

	while (!quit) {
		SDL_WaitEvent(&e);
		if (e.type == SDL_QUIT) {
			quit = true;
		}

		if (e.type == SDL_MOUSEMOTION && e.motion.state != 0) {
			light_c.x = e.motion.x;
			light_c.y = e.motion.y;
		}

		// Reset screen
		SDL_FillRect(surface, &erase_rect, COLOR_BLACK);

		fill_circle(surface, light_c, COLOR_WHITE);
		fill_circle(surface, shadow_c, COLOR_WHITE);

		SDL_UpdateWindowSurface(window);

		SDL_Delay(10);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
