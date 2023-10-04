#ifndef __TEST_HPP__
#define __TEST_HPP__
#include "geometry.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <optional>

class transform {
  // NOTE: We don't support any hierarchy yet
public:
  void translate(glm::vec3 pPosition);
  void rotateX(float pAngle);
  void rotateY(float pAngle);
  void rotateZ(float pAngle);
  void lookAt(glm::vec3 pTarget);
  glm::mat4x4 getMatrix();
};
class material {
public:
  int placeholder;
};
struct mesh {
  std::vector<std::shared_ptr<geometry>> geometries;
  std::vector<std::shared_ptr<material>> materials;
};
class light {
public:
  int placeholder;
};
class camera {
public:
  enum { PERSPECTIVE, ORTHOGRAPHIC } type;
  float near;
  float far;
  float fov;
  glm::mat4x4 getProjection(float pAspect);
};
struct entity_id {
  int id;
  int version;
};
class entity {
private:
  entity_id mId;
  bool mIsValid;

public:
  const entity_id &id() const;
  const bool is_valid() const;

  void dispose();

  // NOTE: We use shared pointers here since we don't know actual types here,
  // and we don't know how it will go
  std::shared_ptr<transform> transform;
  std::shared_ptr<material> material;
  std::shared_ptr<mesh> mesh;
  std::shared_ptr<light> light;
  std::shared_ptr<camera> camera;
};
class entity_store {
public:
  entity &create_entity();
  void remove_entity(const entity_id &pEntityId);
  entity &get_entity(const entity_id &pEntityId) const;
  std::optional<entity &> try_get_entity(const entity_id &pEntityId) const;
  // TODO: Specify valid type
  void begin();
  void end();
};
class world {
  entity_store &get_entity_store() const;
};
class renderer {};

#endif // __TEST_HPP__
