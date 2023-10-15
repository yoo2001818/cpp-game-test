#ifndef __TEST_HPP__
#define __TEST_HPP__
#include "geometry.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <stack>

class transform {
  // NOTE: We don't support any hierarchy yet
public:
  void translate(glm::vec3 pPosition);
  void rotateX(float pAngle);
  void rotateY(float pAngle);
  void rotateZ(float pAngle);
  void lookAt(glm::vec3 pTarget);
  void reset();
  glm::mat4 &getMatrix();
  glm::mat4 getInverseMatrix();

  transform() : mMatrix(1.0){};

private:
  glm::mat4 mMatrix;
};
class material {
public:
  unsigned int mVertexShaderId = -1;
  unsigned int mFragmentShaderId = -1;
  unsigned int mProgramId = -1;
  void prepare();
  void dispose();
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
  glm::mat4 getProjection(float pAspect);
};
struct entity_id {
  int id;
  int version;

  auto operator<=>(const entity_id &) const = default;
};
class entity_store;
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
  std::unique_ptr<::transform> transform;
  std::unique_ptr<::mesh> mesh;
  std::unique_ptr<::light> light;
  std::unique_ptr<::camera> camera;
  std::string name;
  friend entity_store;
};
class entity_store {
public:
  entity &create_entity();
  void remove_entity(const entity_id &pEntityId);
  /**
   * @brief Get the entity using the entity ID.
   *
   * Returns the entity corresponding the entity ID. Returns nullptr if the
   * entity is not available.
   * Note that the pointer should not be freed; the memory allocation must be
   * handled by the entity store itself.
   * Also note that the entity pointer is not persistent. It can move whenever
   * it needs to, currently it's possible when a new entity is created.
   * Hence, the entity pointer must be not stored but re-retrieved using the
   * entity ID.
   * @param pEntityId The entity ID of the entity.
   * @return entity* The pointer to the entity, nullptr if invalid.
   */
  entity *get_entity(const entity_id &pEntityId);
  std::vector<entity>::iterator begin();
  std::vector<entity>::iterator end();

private:
  std::vector<entity> mEntityList;
  std::stack<entity_id> mDeadEntities;
  int mMaxEntityId = 0;
};
class world {
public:
  entity_store &get_entity_store();
  void init();
  void update();
  void render();
  void resize(int pWidth, int pHeight);
  void mouse_move(int x, int y);

private:
  entity_store mEntityStore;
  int mWidth;
  int mHeight;
  float mAngle;
  float mAngleY;
};
class renderer {};

#endif // __TEST_HPP__
