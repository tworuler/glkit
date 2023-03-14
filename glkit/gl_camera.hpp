#ifndef GLKIT_GL_CAMERA_HPP_
#define GLKIT_GL_CAMERA_HPP_

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gl_base.hpp"

namespace glkit {

class Camera {
 public:
  Camera() {
    UpdateViewMat();
    UpdateProjectionMat();
  }

  const Vec3& position() const { return position_; }
  void set_position(const Vec3& position) {
    position_ = position;
    is_view_dirty_ = true;
  }

  float pitch() const { return pitch_; }
  void set_pitch(float pitch) {
    pitch_ = pitch;
    is_view_dirty_ = true;
  }

  float yaw() const { return yaw_; }
  void set_yaw(float yaw) {
    yaw_ = yaw;
    is_view_dirty_ = true;
  }

  float fovy() const { return fovy_; }
  void set_fovy(float fovy) {
    fovy_ = fovy;
    is_projection_dirty_ = true;
  }

  float aspect() const { return aspect_; }
  void set_aspect(float aspect) {
    aspect_ = aspect;
    is_projection_dirty_ = true;
  }

  float near() const { return near_; }
  void set_near(float near) {
    near_ = near;
    is_projection_dirty_ = true;
  }

  float far() const { return far_; }
  void set_far(float far) {
    far_ = far;
    is_projection_dirty_ = true;
  }

  const Vec3& front() const {
    if (is_view_dirty_) UpdateViewMat();
    return front_;
  }

  const Vec3& right() const {
    if (is_view_dirty_) UpdateViewMat();
    return right_;
  }

  const Vec3& up() const {
    if (is_view_dirty_) UpdateViewMat();
    return up_;
  }

  const Mat4& view_mat() const {
    if (is_view_dirty_) UpdateViewMat();
    return view_mat_;
  }

  const Mat4& projection_mat() const {
    if (is_projection_dirty_) UpdateProjectionMat();
    return projection_mat_;
  }

  void MoveFront(float d) { set_position(position() + front() * d); }
  void MoveRight(float d) { set_position(position() + right() * d); }
  void MoveUp(float d) { set_position(position() + up() * d); }

  void TurnUp(float d) { set_pitch(pitch() + d); }
  void TurnDown(float d) { TurnUp(-d); }
  void TurnLeft(float d) { TurnRight(-d); }
  void TurnRight(float d) { set_yaw(yaw() + d); }

 private:
  void UpdateViewMat() const {
    front_.x = cos(pitch_) * cos(yaw_);
    front_.y = sin(pitch_);
    front_.z = cos(pitch_) * sin(yaw_);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
    view_mat_ = glm::lookAt(position_, position_ + front_, up_);
    is_view_dirty_ = false;
  }

  void UpdateProjectionMat() const {
    projection_mat_ = glm::perspective(fovy_, aspect_, near_, far_);
    is_projection_dirty_ = false;
  }

  Vec3 position_ = Vec3(0.f, 0.f, 1.f);
  float pitch_ = 0.f;                    // x in [-PI/2, PI/2]
  float yaw_ = -90.f / 180.f * PI;       // y in [-PI, PI]
  Vec3 world_up_ = Vec3(0.f, 1.f, 0.f);  // normalized vector

  float fovy_ = 45.f / 180.f * PI;
  float aspect_ = 16.f / 9.f;  // widht / hieght
  float near_ = 0.1f;
  float far_ = 100.f;

  mutable bool is_view_dirty_ = true;
  mutable bool is_projection_dirty_ = true;
  mutable Vec3 front_;
  mutable Vec3 right_;
  mutable Vec3 up_;
  mutable Mat4 view_mat_;
  mutable Mat4 projection_mat_;
};

}  // namespace glkit

#endif  // GLKIT_GL_CAMERA_HPP_