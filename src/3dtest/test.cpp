#include "test.hpp"
#include "3dtest/geometry.hpp"
#include <GL/glew.h>
#include <cmath>
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

void material::prepare() {
  if (this->mProgramId == -1) {
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    // FIXME
    const char *vsSource = "#version 330 core\n"
                           "layout (location = 0) in vec3 aPosition;\n"
                           "layout (location = 1) in vec2 aTexCoord;\n"
                           "layout (location = 2) in vec3 aNormal;\n"
                           "layout (location = 3) in vec3 aTangent;\n"
                           "out vec3 vColor;\n"
                           "uniform mat4 uModel;\n"
                           "uniform mat4 uView;\n"
                           "uniform mat4 uProjection;\n"
                           "void main()\n"
                           "{\n"
                           "   gl_Position = uProjection * uView * uModel * "
                           "vec4(aPosition, 1.0);\n"
                           "   vColor = normalize((uView * uModel * "
                           "vec4(aNormal, 0.0)).xyz) * 0.5 + 0.5;\n"
                           "}\0";
    glShaderSource(vs, 1, &vsSource, NULL);
    glCompileShader(vs);

    auto fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fsSource = "#version 330 core\n"
                           "in vec3 vColor;\n"
                           "out vec4 FragColor;\n"
                           "void main()\n"
                           "{\n"
                           "    FragColor = vec4(vColor, 1.0f);\n"
                           "}\0";
    glShaderSource(fs, 1, &fsSource, NULL);
    glCompileShader(fs);

    this->mProgramId = glCreateProgram();
    glAttachShader(this->mProgramId, vs);
    glAttachShader(this->mProgramId, fs);
    glLinkProgram(this->mProgramId);

    glDeleteShader(vs);
    glDeleteShader(fs);
  }
  glUseProgram(this->mProgramId);
}
void material::dispose() {
  if (this->mProgramId != -1) {
    glDeleteProgram(this->mProgramId);
    this->mProgramId = -1;
  }
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
  auto &entity_store = this->get_entity_store();
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
    cube.mesh->geometries.push_back(std::make_shared<geometry>(make_box()));
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
}

void world::update() {
  auto &entity_store = this->get_entity_store();
  this->mAngle += glm::radians(2.0);
  for (auto it = entity_store.begin(); it != entity_store.end(); it++) {
    auto &entity = *it;
    if (entity.name == "cube") {
      entity.transform->rotateY(glm::radians(2.0f));
    }
    if (entity.name == "camera") {
      entity.transform->getMatrix() = glm::mat4(1.0);
      entity.transform->translate(glm::vec3(std::cos(this->mAngle) * 5.0, 0.0,
                                            std::sin(this->mAngle) * 5.0));
      entity.transform->lookAt(glm::vec3(0.0));
    }
  }
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
  for (auto it = entity_store.begin(); it != entity_store.end(); it++) {
    auto &entity = *it;
    if (entity.mesh != nullptr && entity.transform != nullptr) {
      for (int i = 0; i < entity.mesh->geometries.size(); i++) {
        auto &material = entity.mesh->materials[i];
        auto &geometry = entity.mesh->geometries[i];
        material->prepare();
        glUniformMatrix4fv(glGetUniformLocation(material->mProgramId, "uModel"),
                           1, false,
                           glm::value_ptr(entity.transform->getMatrix()));
        glUniformMatrix4fv(
            glGetUniformLocation(material->mProgramId, "uView"), 1, false,
            glm::value_ptr(camera->transform->getInverseMatrix()));
        glUniformMatrix4fv(
            glGetUniformLocation(material->mProgramId, "uProjection"), 1, false,
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
