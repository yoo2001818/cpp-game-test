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

class aabb {
public:
  glm::vec3 min;
  glm::vec3 max;
};
class geometry {
public:
  unsigned int mVbo = -1;
  unsigned int mEbo = -1;
  unsigned int mVao = -1;
  std::vector<glm::vec3> mPositions;
  std::vector<glm::vec2> mTexCoords;
  std::vector<glm::vec3> mNormals;
  std::vector<glm::vec3> mTangents;
  std::vector<unsigned int> mIndices;
  // aabb mBounds;
  int mSize;
  int mType;
  // float mLodThreshold;

  void render();
  void dispose();

private:
  void prepare();
};
geometry make_box();
geometry load_obj(std::string pFilename);

#endif // __GEOMETRY_HPP__
