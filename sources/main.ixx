#include <SDL.h>

import <iostream>;
import <algorithm>;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::printf("SDL could not be initialized! SDL_Error: %s\n",
                SDL_GetError());
    return 0;
  }

  // Create window
  SDL_Window *window = SDL_CreateWindow(
      "Basic C++ SDL project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!window) {
    std::printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
  } else {
    // Create renderer
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
      std::printf("Renderer could not be created! SDL_Error: %s\n",
                  SDL_GetError());
    } else {
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
