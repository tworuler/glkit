#ifndef GLKIT_GL_SHADER_HPP_
#define GLKIT_GL_SHADER_HPP_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "gl_base.hpp"

namespace glkit {

class Shader {
 public:
  Shader() = default;

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

  int InitFromFile(const std::string& vertex_file,
                   const std::string& fragment_file) {
    std::ifstream fin(vertex_file);
    if (!fin.is_open()) {
      LOG(ERROR) << "Failed to open file: " << vertex_file;
      return -1;
    }
    std::stringstream vertex_ss;
    vertex_ss << fin.rdbuf();
    fin.close();
    std::string vertex_src = vertex_ss.str();

    fin.open(fragment_file);
    if (!fin.is_open()) {
      LOG(ERROR) << "Failed to open file: " << fragment_file;
      return -1;
    }
    std::stringstream fragment_ss;
    fragment_ss << fin.rdbuf();
    fin.close();
    std::string fragment_src = fragment_ss.str();

    return Init(vertex_src, fragment_src);
  }

  int Use() const {
    glUseProgram(program_);
    RETURN_IF_GL_ERROR(-1, "glUseProgram");
    return 0;
  }

  int SetInt(const char* name, int value) {
    auto loc = glGetUniformLocation(program_, name);
    LOG_IF(WARN, loc == -1) << "Uniform " << name << " not found";
    glUniform1i(loc, value);
    RETURN_IF_GL_ERROR(-1, "glUniform1i " << name);
    return 0;
  }

  int SetFloat(const char* name, float value) {
    auto loc = glGetUniformLocation(program_, name);
    LOG_IF(WARN, loc == -1) << "Uniform " << name << " not found";
    glUniform1f(loc, value);
    RETURN_IF_GL_ERROR(-1, "glUniform1f " << name);
    return 0;
  }

  int SetVec3(const char* name, const Vec3& value) {
    auto loc = glGetUniformLocation(program_, name);
    LOG_IF(WARN, loc == -1) << "Uniform " << name << " not found";
    glUniform3fv(loc, 1, &value[0]);
    RETURN_IF_GL_ERROR(-1, "glUniform3fv " << name);
    return 0;
  }

  int SetVec3(const char* name, float x, float y, float z) {
    auto loc = glGetUniformLocation(program_, name);
    LOG_IF(WARN, loc == -1) << "Uniform " << name << " not found";
    glUniform3f(loc, x, y, z);
    RETURN_IF_GL_ERROR(-1, "glUniform3f " << name);
    return 0;
  }

  int SetMat4(const char* name, const Mat4& value, bool row_major = false) {
    auto loc = glGetUniformLocation(program_, name);
    LOG_IF(WARN, loc == -1) << "Uniform " << name << " not found";
    glUniformMatrix4fv(loc, 1, row_major, &value[0][0]);
    RETURN_IF_GL_ERROR(-1, "glUniformMatrix4fv " << name);
    return 0;
  }

  void Free() {
    if (program_ != 0) {
      glDeleteProgram(program_);
      program_ = 0;
    }
  }

  ~Shader() { Free(); }

 private:
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

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