#ifndef GLKIT_GL_BASE_HPP_
#define GLKIT_GL_BASE_HPP_

#include <math.h>
#ifdef _WIN32
#include <glad/glad.h>
#endif

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#if CGL_VERSION_1_3
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif  // CGL_VERSION_1_3
#endif

#ifndef LOG
#define LOG(x) std::cerr << std::endl
#endif

#define CHECK_GL_ERROR(msg)                                               \
  do {                                                                    \
    GLenum error = glGetError();                                          \
    if (error != GL_NO_ERROR) {                                           \
      LOG(ERROR) << msg << ": GL error: 0x" << std::uppercase << std::hex \
                 << error;                                                \
    }                                                                     \
  } while (0)

#if defined(__ANDROID__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)
#define VERSION_HEADER "\n"
#else
#define VERSION_HEADER "#version 330 core\n"
#endif

#define VERTEX_HEADER       \
  VERSION_HEADER            \
  "#if __VERSION__ < 130\n" \
  "#define in attribute\n"  \
  "#define out varying\n"   \
  "#endif  // __VERSION__ < 130\n"

#define FRAGMENT_HEADER               \
  VERSION_HEADER                      \
  "#if __VERSION__ < 130\n"           \
  "  #define in varying\n"            \
  "  #define texture texture2D\n"     \
  "#endif  // __VERSION__ < 130\n"    \
  "#ifdef GL_ES\n"                    \
  "  precision highp float;\n"        \
  "#else\n"                           \
  "  #define lowp\n"                  \
  "  #define mediump\n"               \
  "  #define highp\n"                 \
  "  #define texture2D texture\n"     \
  "  out vec4 frag_out; \n"           \
  "  #define gl_FragColor frag_out\n" \
  "#endif  // defined(GL_ES)\n"

#define __STRINGIFY(_x) #_x
#define VERTEX_SHADER(code) VERTEX_HEADER __STRINGIFY(code)
#define FRAGMENT_SHADER(code) FRAGMENT_HEADER __STRINGIFY(code)

#if DEBUG
#define GL_DEBUG_LOG(type, object, action)                        \
  do {                                                            \
    GLint log_length = 0;                                         \
    glGet##type##iv(object, GL_INFO_LOG_LENGTH, &log_length);     \
    if (log_length > 0) {                                         \
      GLchar* log = static_cast<GLchar*>(malloc(log_length));     \
      glGet##type##InfoLog(object, log_length, &log_length, log); \
      LOG(INFO) << #type " " action " log:\n" << log;             \
      free(log);                                                  \
    }                                                             \
  } while (0)
#else
#define GL_DEBUG_LOG(type, object, action)
#endif

#define GL_ERROR_LOG(type, object, action)                        \
  do {                                                            \
    GLint log_length = 0;                                         \
    glGet##type##iv(object, GL_INFO_LOG_LENGTH, &log_length);     \
    if (log_length > 0) {                                         \
      GLchar* log = static_cast<GLchar*>(malloc(log_length));     \
      glGet##type##InfoLog(object, log_length, &log_length, log); \
      LOG(ERROR) << #type " " action " log:\n" << log;            \
      free(log);                                                  \
    }                                                             \
  } while (0)

#define GLKIT_USE_GLM
#ifdef GLKIT_USE_GLM
#include <glm/glm.hpp>
#endif  // GLKIT_USE_GLM

namespace glkit {

const float PI = static_cast<float>(acos(-1.0));

#ifdef GLKIT_USE_GLM
using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Mat4 = glm::mat4;
#endif  // GLKIT_USE_GLM

}  // namespace glkit


#endif  // GLKIT_GL_BASE_HPP_
