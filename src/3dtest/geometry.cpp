#include "geometry.hpp"
#include <GL/glew.h>
#include <fstream>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <iostream>
#include <string>
#include <vector>

geometry make_box() {
  return {
      .mPositions =
          {
              // Front
              {-1.0f, -1.0f, 1.0f},
              {1.0f, -1.0f, 1.0f},
              {1.0f, 1.0f, 1.0f},
              {-1.0f, 1.0f, 1.0f},
              // Top
              {-1.0f, 1.0f, 1.0f},
              {1.0f, 1.0f, 1.0f},
              {1.0f, 1.0f, -1.0f},
              {-1.0f, 1.0f, -1.0f},
              // Back
              {1.0f, -1.0f, -1.0f},
              {-1.0f, -1.0f, -1.0f},
              {-1.0f, 1.0f, -1.0f},
              {1.0f, 1.0f, -1.0f},
              // Bottom
              {-1.0f, -1.0f, -1.0f},
              {1.0f, -1.0f, -1.0f},
              {1.0f, -1.0f, 1.0f},
              {-1.0f, -1.0f, 1.0f},
              // Left
              {-1.0f, -1.0f, -1.0f},
              {-1.0f, -1.0f, 1.0f},
              {-1.0f, 1.0f, 1.0f},
              {-1.0f, 1.0f, -1.0f},
              // Right
              {1.0f, -1.0f, 1.0f},
              {1.0f, -1.0f, -1.0f},
              {1.0f, 1.0f, -1.0f},
              {1.0f, 1.0f, 1.0f},
          },
      .mTexCoords =
          {
              {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
              {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
              {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
              {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
              {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
              {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
          },
      .mNormals =
          {
              // Front
              {0.0f, 0.0f, 1.0f},
              {0.0f, 0.0f, 1.0f},
              {0.0f, 0.0f, 1.0f},
              {0.0f, 0.0f, 1.0f},
              // Top
              {0.0f, 1.0f, 0.0f},
              {0.0f, 1.0f, 0.0f},
              {0.0f, 1.0f, 0.0f},
              {0.0f, 1.0f, 0.0f},
              // Back
              {0.0f, 0.0f, -1.0f},
              {0.0f, 0.0f, -1.0f},
              {0.0f, 0.0f, -1.0f},
              {0.0f, 0.0f, -1.0f},
              // Bottom
              {0.0f, -1.0f, 0.0f},
              {0.0f, -1.0f, 0.0f},
              {0.0f, -1.0f, 0.0f},
              {0.0f, -1.0f, 0.0f},
              // Left
              {-1.0f, 0.0f, 0.0f},
              {-1.0f, 0.0f, 0.0f},
              {-1.0f, 0.0f, 0.0f},
              {-1.0f, 0.0f, 0.0f},
              // Right
              {1.0f, 0.0f, 0.0f},
              {1.0f, 0.0f, 0.0f},
              {1.0f, 0.0f, 0.0f},
              {1.0f, 0.0f, 0.0f},
          },
      .mIndices = {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,
                   8,  9,  10, 10, 11, 8,  12, 13, 14, 14, 15, 12,
                   16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20},
      .mType = GL_TRIANGLES,
  };
}

std::vector<std::string> string_split(std::string s, std::string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

geometry load_obj(std::string pFilename) {
  std::ifstream infile(pFilename);
  std::string line;
  std::vector<glm::vec3> vertPos;
  std::vector<glm::vec3> vertNormals;
  std::vector<glm::vec2> vertTexCoords;
  std::vector<glm::vec3> facePos;
  std::vector<glm::vec3> faceNormals;
  std::vector<glm::vec2> faceTexCoords;
  std::vector<unsigned int> faces;
  while (std::getline(infile, line)) {
    // Process each line
    if (line[0] == '#') {
      continue;
    }
    std::vector<std::string> words = string_split(line, " ");
    // o -> Object
    // v -> aPosition
    // vt -> aTexCoord
    // vn -> aNormal
    // vp -> Not sure
    // f -> Indices
    // mtllib
    // usemtl
    // s off -> Normal smoothing
    if (words[0] == "v") {
      // Vertex coords: v 0 0 0
      vertPos.push_back(
          {std::stof(words[1]), std::stof(words[2]), std::stof((words[3]))});
    } else if (words[0] == "vn") {
      // Normals: vn 0 0 0
      vertNormals.push_back(
          {std::stof(words[1]), std::stof(words[2]), std::stof((words[3]))});
    } else if (words[0] == "vt") {
      // TexCoords: vt 0 0
      vertTexCoords.push_back({std::stof(words[1]), std::stof(words[2])});
    } else if (words[0] == "p") {
      // Point
    } else if (words[0] == "l") {
      // Line
    } else if (words[0] == "f") {
      // Face: f 0/0/0 0/0/0 0/0/0
      // Arbitrary amount of points are possible; we must triangluate them
      int startIndex = facePos.size();
      for (int i = 1; i < words.size(); i += 1) {
        std::vector<std::string> segments = string_split(words[i], "/");
        facePos.push_back(vertPos[std::stoi(segments[0]) - 1]);
        if (segments[1].size() > 0) {
          faceTexCoords.push_back(vertTexCoords[std::stoi(segments[1]) - 1]);
        } else {
          faceTexCoords.push_back({});
        }
        faceNormals.push_back(vertNormals[std::stoi(segments[2]) - 1]);
      }
      for (int i = 3; i <= words.size() - 1; i += 1) {
        faces.push_back(startIndex);
        faces.push_back(startIndex + i - 2);
        faces.push_back(startIndex + i - 1);
      }
    } else if (words[0] == "o") {
      // Finalize object if exists; otherwise specify its name
    } else if (words[0] == "usemtl") {
      // mtl file directory path; but we won't do anything for now
    } else if (words[0] == "g") {
      // Put the object in group if exists
    } else if (words[0] == "s") {
      // Smoothing: s off / s 0 / s on / s 1
    }
  }
  geometry geom;
  geom.mIndices = faces;
  geom.mPositions = facePos;
  geom.mNormals = faceNormals;
  geom.mTexCoords = faceTexCoords;
  geom.mType = GL_TRIANGLES;
  return geom;
}

void geometry::prepare() {
  if (this->mVbo == -1) {
    glGenBuffers(1, &(this->mVbo));
    glBindBuffer(GL_ARRAY_BUFFER, this->mVbo);
    // Estimate total size of attributes
    auto size = this->mPositions.size();
    auto byteSize = sizeof(glm::vec3) * size;
    if (!this->mTexCoords.empty()) {
      byteSize += sizeof(glm::vec2) * size;
    }
    if (!this->mNormals.empty()) {
      byteSize += sizeof(glm::vec3) * size;
    }
    if (!this->mTangents.empty()) {
      byteSize += sizeof(glm::vec3) * size;
    }
    // Upload each buffer
    glBufferData(GL_ARRAY_BUFFER, byteSize, nullptr, GL_STATIC_DRAW);
    auto pos = 0;
    glBufferSubData(GL_ARRAY_BUFFER, pos, sizeof(glm::vec3) * size,
                    mPositions.data());
    pos += sizeof(glm::vec3) * size;
    if (!this->mTexCoords.empty()) {
      glBufferSubData(GL_ARRAY_BUFFER, pos, sizeof(glm::vec2) * size,
                      mTexCoords.data());
      pos += sizeof(glm::vec2) * size;
    }
    if (!this->mNormals.empty()) {
      glBufferSubData(GL_ARRAY_BUFFER, pos, sizeof(glm::vec3) * size,
                      mNormals.data());
      pos += sizeof(glm::vec3) * size;
    }
    if (!this->mTangents.empty()) {
      glBufferSubData(GL_ARRAY_BUFFER, pos, sizeof(glm::vec3) * size,
                      mTangents.data());
      pos += sizeof(glm::vec3) * size;
    }
    this->mSize = size;
  }
  if (this->mEbo == -1 && !this->mIndices.empty()) {
    glGenBuffers(1, &(this->mEbo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int) * this->mIndices.size(),
                 this->mIndices.data(), GL_STATIC_DRAW);
  }
  if (this->mVao == -1) {
    glGenVertexArrays(1, &(this->mVao));
    glBindVertexArray(this->mVao);
    glBindBuffer(GL_ARRAY_BUFFER, this->mVbo);
    auto pos = 0;
    auto size = this->mPositions.size();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (void *)0);
    glEnableVertexAttribArray(0);
    pos += sizeof(glm::vec3) * size;
    if (!this->mTexCoords.empty()) {
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2),
                            (void *)pos);
      glEnableVertexAttribArray(1);
      pos += sizeof(glm::vec2) * size;
    }
    if (!this->mNormals.empty()) {
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                            (void *)pos);
      glEnableVertexAttribArray(2);
      pos += sizeof(glm::vec3) * size;
    }
    if (!this->mTangents.empty()) {
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                            (void *)pos);
      glEnableVertexAttribArray(2);
      pos += sizeof(glm::vec3) * size;
    }
    this->mSize = size;
  }
}

void geometry::render() {
  this->prepare();
  glBindVertexArray(this->mVao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mEbo);
  if (this->mIndices.empty()) {
    glDrawArrays(this->mType, 0, this->mSize);
  } else {
    glDrawElements(this->mType, this->mIndices.size(), GL_UNSIGNED_INT, 0);
  }
}

void geometry::dispose() {
  if (this->mVao != -1) {
    glDeleteVertexArrays(1, &(this->mVao));
    this->mVao = -1;
  }
  if (this->mVbo != -1) {
    glDeleteBuffers(1, &(this->mVbo));
    this->mVbo = -1;
  }
  if (this->mEbo != -1) {
    glDeleteBuffers(1, &(this->mEbo));
    this->mEbo = -1;
  }
}

void calc_normals(geometry &pGeometry) {
  auto &normals = pGeometry.mNormals;
  auto &pos = pGeometry.mPositions;
  auto &indices = pGeometry.mIndices;
  normals.clear();
  normals.resize(pos.size());
  for (int vertId = 0; vertId < normals.size(); vertId += 1) {
    normals[vertId] = glm::vec3(.0f);
  }
  for (int faceId = 0; faceId < indices.size(); faceId += 3) {
    auto v1 = indices[faceId];
    auto v2 = indices[faceId + 1];
    auto v3 = indices[faceId + 2];
    auto &origin = pos[v1];
    auto p1 = pos[v2] - origin;
    auto p2 = pos[v3] - origin;
    glm::vec3 uv = glm::cross(p1, p2);
    normals[v1] += uv;
    normals[v2] += uv;
    normals[v3] += uv;
  }
  // Normalize vertices
  for (int vertId = 0; vertId < normals.size(); vertId += 1) {
    normals[vertId] = glm::normalize(normals[vertId]);
  }
}

void calc_tangents(geometry &pGeometry) {
  auto &pos = pGeometry.mPositions;
  auto &texCoords = pGeometry.mTexCoords;
  auto &tangents = pGeometry.mTangents;
  auto &indices = pGeometry.mIndices;
  tangents.clear();
  tangents.resize(pos.size());
  for (int faceId = 0; faceId < indices.size(); faceId += 3) {
    auto v1 = indices[faceId];
    auto v2 = indices[faceId + 1];
    auto v3 = indices[faceId + 2];
    auto &origin = pos[v1];
    auto p1 = pos[v2] - origin;
    auto p2 = pos[v3] - origin;
    auto &texOrigin = texCoords[v1];
    auto texP1 = texCoords[v2] - texOrigin;
    auto texP2 = texCoords[v3] - texOrigin;
    auto f = 1.0f / (texP1.x * texP2.y - texP2.x * texP1.y);
    auto &tangent = tangents[v1];
    tangent.x = f * (texP2.y * p1.x - texP1.y * p2.x);
    tangent.y = f * (texP2.y * p1.y - texP1.y * p2.y);
    tangent.z = f * (texP2.y * p1.z - texP1.y * p2.z);
    // Calculate bi-tangent. To save vertex array, it can be calculated in
    // vertex shader; however we have to specify the cross order to get right
    // result. This can be done by using a modifier... I think.
    // To calculate modifier, we have to calculate dot product with
    // bi-tangent from vertex shader and bi-tangent we calculated.
    auto normal = glm::cross(p1, p2);
    normal = glm::normalize(normal);
    auto leftBitangent = glm::cross(glm::vec3(tangent), normal);
    glm::vec3 bitangent;
    bitangent.x = f * (texP2.x * p1.x - texP1.x * p2.x);
    bitangent.y = f * (texP2.x * p1.y - texP1.x * p2.y);
    bitangent.z = f * (texP2.x * p1.z - texP1.x * p2.z);
    auto modifier = glm::dot(bitangent, leftBitangent);
    tangent.w = modifier < 0.0f ? -1.0f : 1.0f;
    tangents[v2] = tangent;
    tangents[v3] = tangent;
  }
}
