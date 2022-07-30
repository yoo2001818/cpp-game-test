#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include "game.hpp"

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
  {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window;

  window = SDL_CreateWindow("CppGameTest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_VULKAN);

  if (window == nullptr)
  {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
  if (renderer == nullptr)
  {
    SDL_Log("Unable to create renderer: %s", SDL_GetError());
    return 1;
  }

  EntityStore entityStore;
  /*
  auto entity = entityStore.create();
  entity->name = "A Test";

  auto entity2 = entityStore.create();
  auto entity3 = entityStore.create();

  for (auto e : entityStore) {
    std::cout << e->id << " - " << e->name << std::endl;
  }
  */

  while (true)
  {
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderClear(renderer);

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowHeight, &windowHeight);

    for (int i = 0; i < 10; i += 1) {
      auto entity = entityStore.create();
      entity->transform.position.x = windowWidth / 2;
      entity->transform.position.y = windowHeight / 2;
    }

    for (auto e : entityStore) {
      e->transform.position.x += 1.0;
      e->transform.position.y += 1.0;
    }

    for (auto e : entityStore) {
      SDL_Rect rect;
      rect.x = e->transform.position.x;
      rect.y = e->transform.position.y;
      rect.w = 5;
      rect.h = 5;
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(12);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
