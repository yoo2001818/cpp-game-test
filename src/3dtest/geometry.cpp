#include "geometry.hpp"
#include <GL/glew.h>
#include <fstream>
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
      break;
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
      for (int i = 1; i < words.size(); i += 1) {
        std::vector<std::string> segments = string_split(words[i], "/");
        facePos.push_back(vertPos[std::stoi(segments[0])]);
        faceNormals.push_back(vertNormals[std::stoi(segments[1])]);
        faceTexCoords.push_back(vertTexCoords[std::stoi(segments[2])]);
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
    /*
  const builder = new GeometryBuilder();
  builder.clearAttributes(['aPosition', 'aNormal', 'aTexCoord'], [3, 3, 2]);
  input.split('\n').forEach((line) => {
    if (line[0] === '#') {
      return;
    }
    const words = line.trim().split(/\s+/g);
    // o -> Object
    // v -> aPosition
    // vt -> aTexCoord
    // vn -> aNormal
    // vp -> Not sure
    // f -> Indices
    // mtllib
    // usemtl
    // s off -> Normal smoothing
    switch (words[0]) {
      case 'v': {
        // Vertex coords: v 0 0 0
        const x = parseFloat(words[1]);
        const y = parseFloat(words[2]);
        const z = parseFloat(words[3]);
        builder.addAttribute(POSITION, [x, y, z]);
        break;
      }
      case 'vn': {
        // Normals: vn 0 0 0
        const x = parseFloat(words[1]);
        const y = parseFloat(words[2]);
        const z = parseFloat(words[3]);
        builder.addAttribute(NORMAL, [x, y, z]);
        break;
      }
      case 'vt': {
        // TexCoords: vt 0 0
        const x = parseFloat(words[1]);
        const y = parseFloat(words[2]);
        builder.addAttribute(TEXCOORD, [x, y]);
        break;
      }
      case 'p':
        // Point
        break;
      case 'l':
        // Line
        break;
      case 'f': {
        // Face: f 0/0/0 0/0/0 0/0/0
        // Arbitrary amount of points are possible; we must triangluate them
        const points = words.slice(1).map((arg) => {
          const [pos, tex, normal] = arg.split('/');
          return [
            parseInt(pos, 10) - 1,
            parseInt(normal, 10) - 1,
            parseInt(tex, 10) - 1,
          ];
        // Instead of making ChannelGeometry here, I could just append
        // individual face's verticies to the buffer
        });
        builder.addFace(points);
        break;
      }
      case 'o': {
        // Finalize object if exists; otherwise specify its name
        if (builder.faces.length > 0) {
          // Finalize the object and convert it to ECS entity
          output.push({
            name: objectName!,
            geometry: builder.toChannelGeometry(),
          });
        }
        objectName = words.slice(1).join(' ');
        builder.clearFaces();
        break;
      }
      case 'usemtl':
        // Nothing yet..
        break;
      case 'g':
        // Put the object in group if exists
        break;
      case 's':
        // Smoothing: s off / s 0 / s on / s 1
        break;
      default:
    }
  });
  // Finalize the object
  if (builder.faces.length > 0) {
    // Finalize the object and convert it to ECS entity
    output.push({
      name: objectName!,
      geometry: builder.toChannelGeometry(),
    });
  }
    */
  }
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
