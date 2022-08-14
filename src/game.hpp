#ifndef GAME_HPP_
#define GAME_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include "world.hpp"

class game {
  public:
  world mWorld;
  SDL_Renderer *mRenderer;

  game(SDL_Renderer *renderer): mWorld(), mRenderer(renderer) {}

  void init();
  void update();
  void render();
  void handleEvent(const SDL_Event& event);
};

#endif // GAME_HPP_
