#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "game.hpp"

int main()
{
  std::srand(std::time(nullptr));

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

  World world;
  /*
  auto entity = world.create();
  entity->name = "A Test";

  auto entity2 = world.create();
  auto entity3 = world.create();

  for (auto e : world) {
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

    uint64_t beginTime = SDL_GetTicks64();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderClear(renderer);

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    for (int i = 0; i < 10; i += 1) {
      auto entity = world.create();
      entity->transform.position.x = windowWidth / 2;
      entity->transform.position.y = windowHeight / 2;
      entity->velocity.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0 - 1.0;
      entity->velocity.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0 - 1.0;
      entity->velocity = glm::normalize(entity->velocity);
    }

    for (auto i = world.begin(); i != world.end(); ++i) {
      auto e = *i;
      e->transform.position.x += e->velocity.x * 2.0;
      e->transform.position.y += e->velocity.y * 2.0;

      if (
        e->transform.position.x < 0.0 ||
        e->transform.position.x > windowWidth ||
        e->transform.position.y < 0.0 ||
        e->transform.position.y > windowHeight
      ) {
        i ++;
        world.remove(e);
      }
    }

    for (auto e : world) {
      SDL_Rect rect;
      rect.x = e->transform.position.x;
      rect.y = e->transform.position.y;
      rect.w = 5;
      rect.h = 5;
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);

    uint64_t endTime = SDL_GetTicks64();
    int32_t deltaTime = static_cast<int32_t>(endTime - beginTime);
    int32_t sleepTime = 12 - deltaTime;

    if (sleepTime > 0) {
      SDL_Delay(sleepTime);
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
