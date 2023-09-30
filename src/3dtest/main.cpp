#include "3dtest/geometry.hpp"
#include "world.hpp"
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

  window =
      SDL_CreateWindow("CppGameTest", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);

  if (window == nullptr) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    return 1;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);
  if (glewInit() != GLEW_OK) {
    return 1;
  }

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
  float colors[] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  unsigned int indices[] = {0, 1, 2};

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), nullptr,
               GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void *)(3 * 3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 vPos;\n"
      "layout (location = 1) in vec3 vColor;\n"
      "out vec3 aColor;\n"
      "void main()\n"
      "{\n"
      "   aColor = vColor;\n"
      "   gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);\n"
      "}\0";

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  const char *fragmentShaderSource = "#version 330 core\n"
                                     "in vec3 aColor;\n"
                                     "out vec4 FragColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "    FragColor = vec4(aColor, 1.0f);\n"
                                     "}\0";

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  world::world mWorld;

  auto mGeom = std::make_shared<render::geometry>(render::make_box());
  auto mMaterial = std::make_shared<world::standard_material>();

  mWorld.mEntities.push_back({});
  world::entity &mEntity = mWorld.mEntities.back();

  mEntity.mMesh = {{mGeom}, {mMaterial}};
  mEntity.mName = "Test";

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

      glUseProgram(shaderProgram);
      glBindVertexArray(vao);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
