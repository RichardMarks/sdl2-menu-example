#include <SDL2/SDL.h>

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 360;

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    throw std::runtime_error("Unable to initialize SDL2: " + std::string(SDL_GetError()));
  }

  SDL_Window *window = SDL_CreateWindow("Menu Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
  if (!window)
  {
    throw std::runtime_error("Unable to create window: " + std::string(SDL_GetError()));
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
  {
    throw std::runtime_error("Unable to create renderer: " + std::string(SDL_GetError()));
  }

  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  bool running = true;
  SDL_Event event;

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = false;
      }
      else if (event.type == SDL_KEYDOWN)
      {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
          running = false;
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 0x30, 0x60, 0x90, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
