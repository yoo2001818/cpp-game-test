#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__
#include "3dtest/geometry.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <stack>

struct light_block {
  glm::vec3 mPosition;
  glm::vec3 mColor;
  float mAttenuation;
};
class material_shader {
public:
  unsigned int mVertexShaderId = -1;
  unsigned int mFragmentShaderId = -1;
  unsigned int mProgramId = -1;
  std::string mVertexShader;
  std::string mFragmentShader;
  void prepare();
  void dispose();
};
class material {
public:
  std::map<int, material_shader> mShaders;
  unsigned int mCurrentProgramId;
  void prepare(const std::vector<light_block> &pLights);
  void dispose();
};

#endif
