#ifndef GLKIT_GL_MESH_MANAGER_HPP_
#define GLKIT_GL_MESH_MANAGER_HPP_

#include <map>
#include <string>
#include <vector>

#include "gl_mesh.hpp"

namespace glkit {

class MeshManager {
 public:
  MeshManager() = default;
  ~MeshManager() = default;

  Mesh* GetMesh(const std::string& name) {
    auto it = meshes_.find(name);
    if (it == meshes_.end()) {
      return nullptr;
    }
    return it->second;
  }

  Mesh* AddMeshFromObjFile(const std::string& name, const std::string& file) {
    auto it = meshes_.find(name);
    if (it != meshes_.end()) {
      LOG(WARN) << "Mesh already exists: " << name;
      return it->second;
    }
    mesh_pool_.emplace_back(new Mesh());
    Mesh* mesh = mesh_pool_.back().get();
    if (mesh->InitFromObjFile(file) != 0) {
      mesh_pool_.pop_back();
      LOG(ERROR) << "Failed to add mesh: " << name;
      return nullptr;
    }
    meshes_[name] = mesh;
    return mesh;
  }

  Mesh* AddMesh(const std::string& name, const std::vector<Vertex>& vertices,
                const std::vector<GLuint>& indices) {
    auto it = meshes_.find(name);
    if (it != meshes_.end()) {
      LOG(WARN) << "Mesh already exists: " << name;
      return it->second;
    }
    mesh_pool_.emplace_back(new Mesh());
    Mesh* mesh = mesh_pool_.back().get();
    if (mesh->Init(vertices, indices) != 0) {
      mesh_pool_.pop_back();
      LOG(ERROR) << "Failed to add mesh: " << name;
      return nullptr;
    }
    meshes_[name] = mesh;
    return mesh;
  }

 private:
  MeshManager(const MeshManager&) = delete;
  MeshManager& operator=(const MeshManager&) = delete;

  std::map<std::string, Mesh*> meshes_;
  std::vector<std::unique_ptr<Mesh>> mesh_pool_;
};
}  // namespace glkit

#endif  // GLKIT_GL_MESH_MANAGER_HPP_