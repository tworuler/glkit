#ifndef GLKIT_GL_SHADER_MANAGER_HPP_
#define GLKIT_GL_SHADER_MANAGER_HPP_

#include <map>
#include <string>
#include <vector>

#include "gl_shader.hpp"

namespace glkit {

class ShaderManager {
 public:
  ShaderManager() = default;
  ~ShaderManager() = default;

  Shader* GetShader(const std::string& name) {
    auto it = shaders_.find(name);
    if (it == shaders_.end()) {
      return nullptr;
    }
    return it->second;
  }

  Shader* AddShaderFromFile(const std::string& name,
                            const std::string& vertex_file,
                            const std::string& fragment_file) {
    auto it = shaders_.find(name);
    if (it != shaders_.end()) {
      LOG(WARN) << "Shader already exists: " << name;
      return it->second;
    }
    shader_pool_.emplace_back(new Shader());
    Shader* shader = shader_pool_.back().get();
    if (shader->InitFromFile(vertex_file, fragment_file) != 0) {
      shader_pool_.pop_back();
      LOG(ERROR) << "Failed to add shader: " << name;
      return nullptr;
    }
    shaders_[name] = shader;
    return shader;
  }

  Shader* AddShader(const std::string& name, const std::string& vertex_src,
                    const std::string& fragment_src) {
    auto it = shaders_.find(name);
    if (it != shaders_.end()) {
      LOG(WARN) << "Shader already exists: " << name;
      return it->second;
    }
    shader_pool_.emplace_back(new Shader());
    Shader* shader = shader_pool_.back().get();
    if (shader->Init(vertex_src, fragment_src) != 0) {
      shader_pool_.pop_back();
      LOG(ERROR) << "Failed to add shader: " << name;
      return nullptr;
    }
    shaders_[name] = shader;
    return shader;
  }

  void Clear() {
    shaders_.clear();
    shader_pool_.clear();
  }

 private:
  ShaderManager(const ShaderManager&) = delete;
  ShaderManager& operator=(const ShaderManager&) = delete;

  std::map<std::string, Shader*> shaders_;
  std::vector<std::unique_ptr<Shader>> shader_pool_;
};

}  // namespace glkit

#endif  // GLKIT_GL_SHADER_MANAGER_HPP_