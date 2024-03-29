#ifndef __RENDER_HPP__
#define __RENDER_HPP__

#include "geometry.hpp"
#include "glm/mat4x4.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace render {
class shader {
public:
  unsigned int mProgram;
  std::string mVertexShader;
  std::string mFragmentShader;
};
class texture {
public:
  unsigned int mTexture;
};
class material {
public:
  virtual void render() = 0;
};
class flat_material : public material {
  glm::vec3 mAlbedo;

  virtual void render();
};
class standard_material : public material {
public:
  glm::vec3 mAlbedo;
  std::shared_ptr<texture> mAlbedoTexture;
  float mMetalic;
  std::shared_ptr<texture> mMetalicTexture;
  float mRoughness;
  std::shared_ptr<texture> mRoughnessTexture;
  std::shared_ptr<texture> mNormal;
  glm::vec2 mTexScale;

  virtual void render();
};
class mesh {
public:
  std::vector<std::shared_ptr<render::geometry>> mGeometries;
  std::vector<std::shared_ptr<material>> mMaterials;
};
class light {
public:
  glm::vec3 mColor;
  float intensity;
};
class camera {
public:
  glm::mat4x4 mView;
};
class transform {
public:
  glm::mat4x4 mTransform;
};
};     // namespace render
#endif // __RENDER_HPP__
