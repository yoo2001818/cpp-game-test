#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_video.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

int main() {
  std::srand(std::time(nullptr));

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window;

  window =
      SDL_CreateWindow("CppGameTest", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);

  if (window == nullptr) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    return 1;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);

  {
    while (true) {
      SDL_Event event;
      if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          break;
        }
      }

      uint64_t beginTime = SDL_GetTicks64();

      // Update
      glViewport(0, 0, 1024, 768);
      glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT);

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
