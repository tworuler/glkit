#ifndef GLKIT_GL_SHADER_HPP_
#define GLKIT_GL_SHADER_HPP_

#include <iostream>
#include <string>

#include "gl_base.hpp"

namespace glkit {

class Shader {
 public:
  int Init(const std::string& vertex_src, const std::string& fragment_src) {
    int ret = 0;
    GLuint vertex_shader = 0;
    ret = CompileShader(GL_VERTEX_SHADER, vertex_src, &vertex_shader);
    if (ret != 0) return ret;
    GLuint fragment_shader = 0;
    ret = CompileShader(GL_FRAGMENT_SHADER, fragment_src, &fragment_shader);
    if (ret != 0) return ret;
    ret = CreateShaderProgram(vertex_shader, fragment_shader, &program_);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return ret;
  }

  void Use() {
    glUseProgram(program_);
    CHECK_GL_ERROR("glUseProgram");
  }

  void SetInt(const char* name, int value) {
    auto loc = glGetUniformLocation(program_, name);
    glUniform1i(loc, value);
    CHECK_GL_ERROR("glUniform1i");
  }

  void SetFloat(const char* name, float value) {
    auto loc = glGetUniformLocation(program_, name);
    glUniform1f(loc, value);
    CHECK_GL_ERROR("glUniform1f");
  }

  void SetVec3(const char* name, const Vec3& value) {
    auto loc = glGetUniformLocation(program_, name);
    glUniform3fv(loc, 1, &value[0]);
    CHECK_GL_ERROR("glUniform3fv");
  }

  void SetVec3(const char* name, float x, float y, float z) {
    auto loc = glGetUniformLocation(program_, name);
    glUniform3f(loc, x, y, z);
    CHECK_GL_ERROR("glUniform3f");
  }

  void SetMat4(const char* name, const Mat4& value,
               bool row_major = false) {
    auto loc = glGetUniformLocation(program_, name);
    glUniformMatrix4fv(loc, 1, row_major, &value[0][0]);
    CHECK_GL_ERROR("glUniformMatrix4fv");
  }

 private:
  int CompileShader(GLenum shader_type, const std::string& shader_source,
                    GLuint* shader) {
    *shader = glCreateShader(shader_type);
    if (*shader == 0) {
      LOG(ERROR) << "Unable to create shader of type: " << shader_type;
      return -1;
    }
    const char* shader_source_cstr = shader_source.c_str();
    glShaderSource(*shader, 1, &shader_source_cstr, NULL);
    glCompileShader(*shader);

    GLint compiled;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      LOG(ERROR) << "Unable to compile shader:\n" << shader_source;
      GL_ERROR_LOG(Shader, *shader, "compile");
      glDeleteShader(*shader);
      *shader = 0;
      return -1;
    }
    return 0;
  }

  int CreateShaderProgram(GLuint vertex_shader, GLuint fragment_shader,
                          GLuint* shader_program) {
    *shader_program = glCreateProgram();
    if (*shader_program == 0) {
      LOG(ERROR) << "Unable to create shader program";
      return -1;
    }
    glAttachShader(*shader_program, vertex_shader);
    glAttachShader(*shader_program, fragment_shader);

    glLinkProgram(*shader_program);

    GLint is_linked = 0;
    glGetProgramiv(*shader_program, GL_LINK_STATUS, &is_linked);
    if (!is_linked) {
      LOG(ERROR) << "Unable to link shader program";
      glDeleteProgram(*shader_program);
      *shader_program = 0;
      return -1;
    }
    return 0;
  }

  GLuint program_ = 0;
};

}  // namespace glkit

#endif  // GLKIT_GL_SHADER_HPP_