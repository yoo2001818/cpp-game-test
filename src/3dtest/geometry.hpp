#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

#include "glm/mat4x4.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace render {
class aabb {
public:
  glm::vec3 min;
  glm::vec3 max;
};
class geometry {
public:
  unsigned int mVbo;
  unsigned int mVao;
  std::vector<glm::vec3> mPositions;
  std::vector<glm::vec2> mTexCoords;
  std::vector<glm::vec3> mNormals;
  std::vector<glm::vec3> mTangents;
  // aabb mBounds;
  // int mPolyCount;
  int mType;
  // float mLodThreshold;
};
geometry make_box();
} // namespace render

#endif // __GEOMETRY_HPP__
