#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include "glm/mat4x4.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace world {
class entity;
class world {
public:
  std::vector<entity> mEntities;
};
typedef glm::mat4x4 transform;
class aabb {
public:
  glm::vec3 min;
  glm::vec3 max;
};
class geometry {
public:
  unsigned int mVbo;
  std::vector<glm::vec3> mPositions;
  std::vector<glm::vec2> mTexCoords;
  std::vector<glm::vec3> mNormals;
  std::vector<glm::vec3> mTangents;
  aabb mBounds;
  int mPolyCount;
  int mType;
  float mLodThreshold;
  std::map<int, unsigned int> mVaos;
};
class shader {
public:
  unsigned int mProgram;
  std::string mVertexShader;
  std::string mFragmentShader;
};
class material {
public:
  std::shared_ptr<shader> mShader;
  // TODO ???
  std::map<std::string, void *> mUniforms;
};
class mesh {
public:
  std::vector<std::vector<std::shared_ptr<geometry>>> mGeometries;
  std::vector<std::shared_ptr<material>> mMaterials;
};
class light {
public:
};
class camera {
public:
  glm::mat4x4 mView;
};
class entity {
public:
  std::vector<entity> mChildren;
  transform mTransform;
  std::string mName;
  std::optional<mesh> mMesh;
  std::optional<light> mLight;
  std::optional<camera> mCamera;
};
}; // namespace world

#endif // __WORLD_HPP__
