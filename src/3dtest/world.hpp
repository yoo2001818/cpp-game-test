#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include "glm/mat4x4.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
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
class geometry {
public:
  unsigned int mVbo;
  std::vector<glm::vec3> mPosition;
  std::vector<glm::vec2> mTexCoord;
  std::vector<glm::vec3> mNormal;
};
class material {
public:
  unsigned int mProgram;
};
class mesh {
public:
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
