#include <SDL.h>

import <iostream>;
import <algorithm>;
import <cmath>;

constexpr int SCREEN_WIDTH = 900;
constexpr int SCREEN_HEIGHT = 600;
constexpr int COLOR_WHITE = 0xffffff;

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
	auto circle = Circle{ 200, 200, 80 };

	fill_circle(surface, circle, COLOR_WHITE);

	SDL_UpdateWindowSurface(window);

	SDL_Delay(5000);

	return 0;
}

int main_old(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::printf("SDL could not be initialized! SDL_Error: %s\n",
			SDL_GetError());
		return 0;
	}

	// Create window
	SDL_Window* window = SDL_CreateWindow("Basic C++ SDL project",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (!window) {
		std::printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		// Create renderer
		SDL_Renderer* renderer =
			SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		if (!renderer) {
			std::printf("Renderer could not be created! SDL_Error: %s\n",
				SDL_GetError());
		}
		else {
			// Declare rect of square
			SDL_Rect squareRect;

			// Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
			squareRect.w = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
			squareRect.h = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

			// Square position: In the middle of the screen
			squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
			squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;

			// Event loop exit flag
			bool quit = false;

			// Event loop
			while (!quit) {
				SDL_Event e;

				// Wait indefinitely for the next available event
				SDL_WaitEvent(&e);

				// User requests quit
				if (e.type == SDL_QUIT) {
					quit = true;
				}

				// Initialize renderer color white for the background
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Clear screen
				SDL_RenderClear(renderer);

				// Set renderer color red to draw the square
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

				// Draw filled square
				SDL_RenderFillRect(renderer, &squareRect);

				// Update screen
				SDL_RenderPresent(renderer);
			}

			// Destroy renderer
			SDL_DestroyRenderer(renderer);
		}

		// Destroy window
		SDL_DestroyWindow(window);
	}

	// Quit SDL
	SDL_Quit();

	return 0;
}