#include "test.hpp"
#include <functional>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/matrix.hpp>

void transform::translate(glm::vec3 pPosition) {
  this->mMatrix = glm::translate(this->mMatrix, pPosition);
}
void transform::rotateX(float pAngle) {
  glm::rotate(this->mMatrix, pAngle, glm::vec3(1.0f, 0.0f, 0.0f));
}
void transform::rotateY(float pAngle) {
  glm::rotate(this->mMatrix, pAngle, glm::vec3(0.0f, 1.0f, 0.0f));
}
void transform::rotateZ(float pAngle) {
  glm::rotate(this->mMatrix, pAngle, glm::vec3(0.0f, 0.0f, 1.0f));
}
void transform::lookAt(glm::vec3 pTarget) {
  glm::vec4 eyePos = this->mMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  glm::lookAt(glm::vec3(eyePos), pTarget, glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 &transform::getMatrix() { return this->mMatrix; }
glm::mat4 transform::getInverseMatrix() { return glm::inverse(this->mMatrix); }

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
  this->material.reset();
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
