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

  if (window == NULL)
  {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    return 1;
  }

  EntityStore entityStore;
  Entity& entity = entityStore.create();
  entity.name = "A Test";

  Entity& entity2 = entityStore.create();
  Entity& entity3 = entityStore.create();

  for (Entity& e : entityStore) {
    std::cout << e.id << " - " << e.name << std::endl;
  }

  SDL_Delay(3000);

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
