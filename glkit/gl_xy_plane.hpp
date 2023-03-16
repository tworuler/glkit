#ifndef GLKIT_GL_XY_PLANE_HPP_
#define GLKIT_GL_XY_PLANE_HPP_

#include <vector>

#include "gl_base.hpp"
#include "gl_shader.hpp"

namespace glkit {

class XyPlane {
 public:
  void Init(int size = 100) {
    size_ = size;
    shader_.Init(vs_, fs_);

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

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), v.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
  }

  void Draw(const Mat4& mvp) {
    shader_.Use();
    shader_.SetMat4("mvp", mvp);
    glBindVertexArray(vao_);
    glDrawArrays(GL_LINES, 0, size_ * 8);
  }

 private:
  int size_ = 0;
  Shader shader_;
  GLuint vao_ = 0;

  // clang-format off
  const char* vs_ = VERTEX_SHADER(
      uniform mat4 mvp;
      in vec3 pos;
      out vec3 p; 

      void main() {
        p = pos;
        gl_Position = mvp * vec4(pos, 1.0);
      }
  );

  const char* fs_ = FRAGMENT_SHADER(
      in vec3 p;

      void main() {
        if (p.x == 0) {
          gl_FragColor = vec4(0.3, 0.54, 0.15, 1.0);
        } else if (p.y == 0) {
          gl_FragColor = vec4(0.56, 0.24, 0.28, 1.0);
        } else {
          gl_FragColor = vec4(0.3, 0.3, 0.3, 1.0);
        }
      }
  );
  // clang-format on
};

}  // namespace glkit

#endif  // GLKIT_GL_XY_PLANE_HPP_
