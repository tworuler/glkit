#ifndef GLKIT_GL_XY_PLANE_HPP_
#define GLKIT_GL_XY_PLANE_HPP_

#include <vector>

#include "gl_base.hpp"
#include "gl_shader.hpp"

namespace glkit {

class XyPlane {
 public:
  XyPlane() = default;

  void Init(Shader* shader, int size = 100) {
    shader_ = shader;
    size_ = size;

    std::vector<float> v;
    for (int x = -size; x < size; x++) {
      v.push_back(static_cast<float>(x));
      v.push_back(static_cast<float>(-size));
      v.push_back(static_cast<float>(x));
      v.push_back(static_cast<float>(size));
    }
    for (int y = -size; y < size; y++) {
      v.push_back(static_cast<float>(-size));
      v.push_back(static_cast<float>(y));
      v.push_back(static_cast<float>(size));
      v.push_back(static_cast<float>(y));
    }

    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), v.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
  }

  int Draw(const Mat4& mvp) {
    int ret = 0;
    ret = shader_->Use();
    if (ret != 0) {
      LOG(ERROR) << "Failed to use shader";
      return ret;
    }

    ret = shader_->SetMat4("mvp", mvp);
    if (ret != 0) {
      LOG(ERROR) << "Failed to set mvp";
      return ret;
    }

    glBindVertexArray(vao_);
    glDrawArrays(GL_LINES, 0, size_ * 8);
    glBindVertexArray(0);
    RETURN_IF_GL_ERROR(-1, "Failed to draw xy plane");
    return ret;
  }

  void Free() {
    if (vao_ != 0) {
      glDeleteVertexArrays(1, &vao_);
      vao_ = 0;
    }
    if (vbo_ != 0) {
      glDeleteBuffers(1, &vbo_);
      vbo_ = 0;
    }
  }

  ~XyPlane() { Free(); }

 private:
  XyPlane(const XyPlane&) = delete;
  const XyPlane& operator=(const XyPlane&) = delete;

  int size_ = 0;
  Shader* shader_ = nullptr;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
};

}  // namespace glkit

#endif  // GLKIT_GL_XY_PLANE_HPP_
