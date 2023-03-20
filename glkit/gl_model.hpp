#ifndef GLKIT_GL_MODEL_HPP_
#define GLKIT_GL_MODEL_HPP_

#include "gl_mesh.hpp"
#include "gl_shader.hpp"

namespace glkit {

enum RenderMode {
  kRenderModeLight = 0,
  kRenderModeDepth = 1,
};

class Model {
 public:
  Model() = default;
  ~Model() = default;

  int Init(Mesh* mesh, Shader* shader, bool is_light = false) {
    mesh_ = mesh;
    shader_ = shader;
    is_light_ = is_light;
    return 0;
  }

  int Draw(const Mat4& view, const Mat4& projection) const {
    shader_->Use();
    shader_->SetMat4("view", view);
    shader_->SetMat4("projection", projection);
    const auto& model = GetModelMatrix();
    shader_->SetMat4("model", model);
    shader_->SetVec3("color", color_);
    if (!is_light_) {
      shader_->SetInt("render_mode", render_mode_);
      shader_->SetFloat("near", near_);
      shader_->SetFloat("far", far_);
    }
    mesh_->Draw(shader_);
    return 0;
  }

  Mat4 GetModelMatrix() const {
    Mat4 model(1.0f);
    model = glm::translate(model, position_);
    model = glm::rotate(model, rotation_.x, Vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation_.y, Vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation_.z, Vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale_);
    return model;
  }

  void SetLight(const Vec3& light_pos, const Vec3& light_color) {
    shader_->Use();
    shader_->SetVec3("light_pos", light_pos);
    shader_->SetVec3("light_color", light_color);
  }

  Vec3 position() const { return position_; }
  void set_position(const Vec3& position) { position_ = position; }

  Vec3 rotation() const { return rotation_; }
  void set_rotation(const Vec3& rotation) { rotation_ = rotation; }

  Vec3 scale() const { return scale_; }
  void set_scale(const Vec3& scale) { scale_ = scale; }

  bool is_light() const { return is_light_; }

  Vec3 color() const { return color_; }
  void set_color(const Vec3& color) { color_ = color; }

  RenderMode render_mode() const { return render_mode_; }
  void set_render_mode(RenderMode render_mode) { render_mode_ = render_mode; }

  void set_near(float near) { near_ = near; }
  void set_far(float far) { far_ = far; }

 private:
  Mesh* mesh_ = nullptr;
  Shader* shader_ = nullptr;
  Vec3 position_ = Vec3(0.0f, 0.0f, 0.0f);
  Vec3 rotation_ = Vec3(0.0f, 0.0f, 0.0f);
  Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f);

  bool is_light_ = false;
  Vec3 color_ = Vec3(1.0f, 1.0f, 1.0f);

  RenderMode render_mode_ = kRenderModeLight;
  float near_;
  float far_;
};

}  // namespace glkit

#endif  // GLKIT_GL_MODEL_HPP_