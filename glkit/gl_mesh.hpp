#ifndef GLKIT_GL_MESH_HPP_
#define GLKIT_GL_MESH_HPP_

#include <fstream>
#include <sstream>
#include <vector>

#include "gl_base.hpp"
#include "gl_shader.hpp"

namespace glkit {

struct Vertex {
  Vec3 position;
  Vec3 normal;
  Vec2 texcoord;
};

class Mesh {
 public:
  int Init(const std::vector<Vertex>& vertices,
           const std::vector<GLuint>& indices) {
    vertices_ = vertices;
    indices_ = indices;

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex),
                 vertices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint),
                 indices_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, texcoord));
    glBindVertexArray(0);

    return 0;
  }

  int InitFromObjFile(const std::string& file_path) {
    std::ifstream fin(file_path);
    if (!fin.is_open()) {
      LOG(ERROR) << "Failed to open file: " << file_path;
      return -1;
    }

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    while (!fin.eof()) {
      std::string line;
      std::getline(fin, line);
      std::stringstream ss(line);
      std::string type;
      ss >> type;
      if (type == "v") {
        Vertex vertex;
        ss >> vertex.position.x >> vertex.position.y >> vertex.position.z;
        vertex.normal = Vec3(0.0f);
        vertices.push_back(vertex);
      } else if (type == "f") {
        std::vector<GLuint> face;
        std::string index;
        while (ss >> index) {
          int pos = index.find('/');
          int idx = std::stoi(index.substr(0, pos));
          face.push_back(idx - 1);
        }
        for (size_t i = 1; i < face.size() - 1; ++i) {
          indices.push_back(face[0]);
          indices.push_back(face[i]);
          indices.push_back(face[i + 1]);
        }
      }
    }

    for (size_t i = 0; i < indices.size(); i += 3) {
      Vec3 v1 = vertices[indices[i]].position;
      Vec3 v2 = vertices[indices[i + 1]].position;
      Vec3 v3 = vertices[indices[i + 2]].position;
      Vec3 normal = glm::cross(v2 - v1, v3 - v1);
      vertices[indices[i]].normal += normal;
      vertices[indices[i + 1]].normal += normal;
      vertices[indices[i + 2]].normal += normal;
    }
    for (size_t i = 0; i < vertices.size(); ++i) {
      vertices[i].normal = glm::normalize(vertices[i].normal);
    }

    int ret = Init(vertices, indices);
    return ret;
  }

  void Draw(const Shader& shader) {
    shader.Use();
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT,
                   static_cast<void*>(0));
    glBindVertexArray(0);
  }

 private:
  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
  GLuint ebo_ = 0;
};

}  // namespace glkit

#endif  // GLKIT_GL_MESH_HPP_