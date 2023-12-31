#include "3dtest/material.hpp"
#include "3dtest/geometry.hpp"
#include <GL/glew.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <numbers>
#include <sstream>

void material_shader::prepare() {
  if (this->mProgramId == -1) {
    int success;
    char infoLog[512];
    auto vsSource = this->mVertexShader.data();
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSource, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vs, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    };

    auto fsSource = this->mFragmentShader.data();
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSource, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fs, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    };

    this->mProgramId = glCreateProgram();
    glAttachShader(this->mProgramId, vs);
    glAttachShader(this->mProgramId, fs);
    glLinkProgram(this->mProgramId);
    glGetProgramiv(this->mProgramId, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(this->mProgramId, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
  }
  glUseProgram(this->mProgramId);
}

void material_shader::dispose() {
  if (this->mProgramId != -1) {
    glDeleteProgram(this->mProgramId);
    this->mProgramId = -1;
  }
}

void material::prepare(const std::vector<light_block> &pLights) {
  auto match = this->mShaders.find(pLights.size());
  if (match == this->mShaders.end()) {
    material_shader shader;
    std::ifstream vsFile("res/phong.vert");
    std::stringstream vsStream;
    vsStream << vsFile.rdbuf();
    std::ifstream fsFile("res/phong.frag");
    std::stringstream fsStream;
    fsStream << fsFile.rdbuf();
    shader.mVertexShader = vsStream.str();
    shader.mFragmentShader = fsStream.str();
    auto [iter, created] = this->mShaders.insert({pLights.size(), shader});
    iter->second.prepare();
    this->mCurrentProgramId = iter->second.mProgramId;
  } else {
    match->second.prepare();
    this->mCurrentProgramId = match->second.mProgramId;
  }
  // Upload uniforms according to parameters
  glm::vec3 color(0.8, 0.8, 0.8);
  float roughness = 0.5;
  float metalic = 0.0;
  glUniform3fv(glGetUniformLocation(this->mCurrentProgramId, "uColor"), 1,
               glm::value_ptr(color));
  glUniform1fv(glGetUniformLocation(this->mCurrentProgramId, "uRoughness"), 1,
               &roughness);
  glUniform1fv(glGetUniformLocation(this->mCurrentProgramId, "uMetalic"), 1,
               &metalic);
  int i = 0;
  for (auto iter = pLights.begin(); iter != pLights.end(); iter++) {
    auto &light = *iter;
    glm::vec4 position(light.mPosition, light.mAttenuation);
    glUniform4fv(
        glGetUniformLocation(this->mCurrentProgramId, "uLightPositions") + i, 1,
        glm::value_ptr(position));
    glUniform3fv(glGetUniformLocation(this->mCurrentProgramId, "uLightColors") +
                     i,
                 1, glm::value_ptr(light.mColor));
    i += 1;
  }
}
void material::dispose() {
  for (auto iter = this->mShaders.begin(); iter != this->mShaders.end();
       iter++) {
    iter->second.dispose();
  }
  this->mShaders.clear();
}
