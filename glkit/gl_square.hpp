#ifndef GLKIT_GL_SQUARE_HPP_
#define GLKIT_GL_SQUARE_HPP_

#include "gl_base.hpp"
#include "gl_shader.hpp"

namespace glkit {

class Square {
 public:
  int Init() {
    // clang-format off
    const char* vs = VERTEX_SHADER(
      uniform mat4 mvp;

      in vec3 pos;

      void main() {
        gl_Position = mvp * vec4(pos, 1.0);
      }
    );

    const char* fs = FRAGMENT_SHADER(
      void main() {
        gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
      }
    );

    // clang-format on
    shader_.Init(vs, fs);

    float vertices[] = {
        -1.0f, -1.0f, 0.0f,  // left bottom
        -1.0f, 1.0f,  0.0f,  // left top
        1.0f,  -1.0f, 0.0f,  // right bottom
        1.0f,  1.0f,  0.0f,  // right top
    };
    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    return 0;
  }

  void Draw(const Mat4& mvp) {
    shader_.Use();
    shader_.SetMat4("mvp", mvp);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
  }

  void Free() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    shader_.Free();
  }

  ~Square() { Free(); }

 private:
  Shader shader_;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
};

}  // namespace glkit

#endif  // GLKIT_GL_SQUARE_HPP_