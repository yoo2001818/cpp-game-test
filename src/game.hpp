#ifndef GAME_HPP_
#define GAME_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include "world.hpp"
#include "viewport.hpp"
#include "resource.hpp"

class game {
  public:
  world mWorld;
  viewport mViewport;
  bool mPaused;
  bool mShouldStep;
  SDL_Renderer *mRenderer;
  resource::resource_manager<SDL_Texture*> mTileResourceManager;

  game(SDL_Renderer *renderer): mWorld(), mRenderer(renderer), mPaused(false), mShouldStep(false) {}

  void init();
  void update();
  void render();
  void handleEvent(const SDL_Event& event);
};

#endif // GAME_HPP_
