#include "test.hpp"
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

void transform::translate(glm::vec3 pPosition) {
  this->mMatrix = glm::translate(this->mMatrix, pPosition);
}
void transform::rotateX(float pAngle) {
  this->mMatrix =
      glm::rotate(this->mMatrix, pAngle, glm::vec3(1.0f, 0.0f, 0.0f));
}
void transform::rotateY(float pAngle) {
  this->mMatrix =
      glm::rotate(this->mMatrix, pAngle, glm::vec3(0.0f, 1.0f, 0.0f));
}
void transform::rotateZ(float pAngle) {
  this->mMatrix =
      glm::rotate(this->mMatrix, pAngle, glm::vec3(0.0f, 0.0f, 1.0f));
}
void transform::lookAt(glm::vec3 pTarget) {
  glm::vec3 eyePos =
      glm::vec3(this->mMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  this->mMatrix =
      glm::inverse(glm::lookAt(eyePos, pTarget, glm::vec3(0.0f, 1.0f, 0.0f)));
}
void transform::reset() { this->mMatrix = glm::mat4(1.0); }

glm::mat4 &transform::getMatrix() { return this->mMatrix; }
glm::mat4 transform::getInverseMatrix() { return glm::inverse(this->mMatrix); }

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

glm::mat4 camera::getProjection(float pAspect) {
  switch (this->type) {
  case ORTHOGRAPHIC:
    return glm::ortho(-1.0f, 1.0f, -1.0f / pAspect, 1.0f / pAspect, this->near,
                      this->far);
  case PERSPECTIVE:
    return glm::perspective(this->fov, pAspect, this->near, this->far);
  default:
    return glm::mat4();
  }
}

const entity_id &entity::id() const { return this->mId; }
const bool entity::is_valid() const { return this->mIsValid; }
void entity::dispose() {
  this->transform.reset();
  this->mesh.reset();
  this->light.reset();
  this->camera.reset();
}

entity &entity_store::create_entity() {
  if (this->mDeadEntities.empty()) {
    entity_id id = {.id = this->mMaxEntityId++, .version = 1};
    this->mEntityList.emplace_back();
    auto &entity = this->mEntityList.back();
    entity.mId = id;
    entity.mIsValid = true;
    return entity;
  } else {
    auto id = this->mDeadEntities.top();
    this->mDeadEntities.pop();
    auto &entity = this->mEntityList[id.id];
    entity.mId.version += 1;
    entity.mIsValid = true;
    return entity;
  }
}
void entity_store::remove_entity(const entity_id &pEntityId) {
  auto entity_ref = this->get_entity(pEntityId);
  if (entity_ref == nullptr)
    return;
  entity_ref->mIsValid = false;
  this->mDeadEntities.push(entity_ref->id());
}
entity *entity_store::get_entity(const entity_id &pEntityId) {
  if (pEntityId.id < 0 || pEntityId.id >= this->mEntityList.size()) {
    return nullptr;
  }
  entity *entity_ref = &(this->mEntityList[pEntityId.id]);
  if (!entity_ref->is_valid() || entity_ref->id() != pEntityId) {
    return nullptr;
  }
  return entity_ref;
};
std::vector<entity>::iterator entity_store::begin() {
  return this->mEntityList.begin();
};
std::vector<entity>::iterator entity_store::end() {
  return this->mEntityList.end();
};

entity_store &world::get_entity_store() { return this->mEntityStore; }

void world::init() {
  this->mAngle = 0.0;
  this->mAngleY = 0.0;
  auto &entity_store = this->get_entity_store();
  auto teapot = std::make_shared<geometry>(load_obj("res/teapot.obj"));
  for (int i = 0; i < 10; i += 1) {
    auto &cube = entity_store.create_entity();
    cube.name = "cube";
    cube.transform = std::make_unique<transform>();
    cube.transform->translate(glm::vec3(glm::linearRand(-2.0f, 2.0f),
                                        glm::linearRand(-2.0f, 2.0f),
                                        glm::linearRand(-2.0f, 2.0f)));
    cube.transform->getMatrix() =
        glm::scale(cube.transform->getMatrix(), glm::vec3(0.2));
    cube.mesh = std::make_unique<mesh>();
    cube.mesh->geometries.push_back(teapot);
    cube.mesh->materials.push_back(std::make_shared<material>());
  }
  {
    auto &camera = entity_store.create_entity();
    camera.name = "camera";
    camera.transform = std::make_unique<transform>();
    camera.transform->translate(glm::vec3(0.0, 5.0, 5.0));
    camera.transform->lookAt(glm::vec3(0.0));
    camera.camera = std::make_unique<::camera>();
    camera.camera->type = camera::PERSPECTIVE;
    camera.camera->near = 0.1f;
    camera.camera->far = 100.0f;
    camera.camera->fov = glm::radians(40.0f);
  }
  {
    auto &light = entity_store.create_entity();
    light.name = "light";
    light.transform = std::make_unique<transform>();
    light.transform->translate(glm::vec3(0.0, 0.0, 0.0));
    light.light = std::make_unique<::light>();
    light.light->mColor = glm::vec3(1.0);
  }
}

void world::update() {
  auto &entity_store = this->get_entity_store();
  for (auto it = entity_store.begin(); it != entity_store.end(); it++) {
    auto &entity = *it;
    if (entity.name == "cube") {
      entity.transform->rotateY(glm::radians(2.0f));
    }
    if (entity.name == "camera") {
      entity.transform->getMatrix() = glm::mat4(1.0);

      entity.transform->translate(
          glm::vec3(std::cos(this->mAngle) * std::cos(this->mAngleY) * 5.0,
                    std::sin(this->mAngleY) * 5.0,
                    std::sin(this->mAngle) * std::cos(this->mAngleY) * 5.0));
      entity.transform->lookAt(glm::vec3(0.0));
    }
  }
}

void world::mouse_move(int x, int y) {
  this->mAngle = (float)x / this->mWidth * std::numbers::pi * 2.0;
  this->mAngleY =
      (float)-y / this->mHeight * std::numbers::pi + std::numbers::pi / 2.0;
}

void world::render() {
  auto &entity_store = this->get_entity_store();
  // Grab camera first; this does't look pretty
  entity *camera = nullptr;
  for (auto it = entity_store.begin(); it != entity_store.end(); it++) {
    auto &entity = *it;
    if (entity.camera != nullptr && entity.transform != nullptr) {
      camera = &entity;
    }
  }
  if (camera == nullptr) {
    return;
  }
  std::vector<light_block> lightBlocks;
  for (auto it = entity_store.begin(); it != entity_store.end(); it++) {
    auto &entity = *it;
    if (entity.light != nullptr && entity.transform != nullptr) {
      light_block block;
      block.mColor = entity.light->mColor;
      block.mPosition = glm::vec3(entity.transform->getMatrix()[3]);
      lightBlocks.push_back(block);
    }
  }
  for (auto it = entity_store.begin(); it != entity_store.end(); it++) {
    auto &entity = *it;
    if (entity.mesh != nullptr && entity.transform != nullptr) {
      for (int i = 0; i < entity.mesh->geometries.size(); i++) {
        auto &material = entity.mesh->materials[i];
        auto &geometry = entity.mesh->geometries[i];
        material->prepare(lightBlocks);
        glUniformMatrix4fv(
            glGetUniformLocation(material->mCurrentProgramId, "uModel"), 1,
            false, glm::value_ptr(entity.transform->getMatrix()));
        glUniformMatrix4fv(
            glGetUniformLocation(material->mCurrentProgramId, "uView"), 1,
            false, glm::value_ptr(camera->transform->getInverseMatrix()));
        glUniformMatrix4fv(
            glGetUniformLocation(material->mCurrentProgramId, "uProjection"), 1,
            false,
            glm::value_ptr(camera->camera->getProjection((float)this->mWidth /
                                                         this->mHeight)));
        geometry->render();
      }
    }
  }
}

void world::resize(int pWidth, int pHeight) {
  this->mWidth = pWidth;
  this->mHeight = pHeight;
  glViewport(0, 0, this->mWidth, this->mHeight);
}
