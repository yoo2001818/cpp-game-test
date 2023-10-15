#include "geometry.hpp"
#include "test.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_video.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

int main() {
  std::srand(std::time(nullptr));

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);

  SDL_Window *window;

  window = SDL_CreateWindow("CppGameTest", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 1024, 768,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (window == nullptr) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    return 1;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);
  if (glewInit() != GLEW_OK) {
    return 1;
  }

  world mWorld;
  mWorld.resize(1024, 768);
  mWorld.init();

  {
    while (true) {
      SDL_Event event;
      bool exit = false;
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          exit = true;
          break;
        }
        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_RESIZED) {
          mWorld.resize(event.window.data1, event.window.data2);
        }
        if (event.type == SDL_MOUSEMOTION) {
          mWorld.mouse_move(event.motion.x, event.motion.y);
        }
      }
      if (exit)
        break;

      uint64_t beginTime = SDL_GetTicks64();

      // Update
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);

      mWorld.update();
      mWorld.render();

      SDL_GL_SwapWindow(window);

      uint64_t endTime = SDL_GetTicks64();
      int32_t deltaTime = static_cast<int32_t>(endTime - beginTime);
      int32_t sleepTime = 12 - deltaTime;

      if (sleepTime > 0) {
        SDL_Delay(sleepTime);
      }
    }
  }

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
