#ifndef GLKIT_GL_CAMERA_HPP_
#define GLKIT_GL_CAMERA_HPP_

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

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
    UpdateViewMat();
  }

  const Vec3& rotation() const { return rotation_; }
  void set_rotation(const Vec3& rotation) {
    rotation_ = rotation;
    UpdateViewMat();
  }

  float fovy() const { return fovy_; }
  void set_fovy(float fovy) {
    UpdateProjectionMat();
    fovy_ = fovy;
  }

  float aspect() const { return aspect_; }
  void set_aspect(float aspect) {
    UpdateProjectionMat();
    aspect_ = aspect;
  }

  float near() const { return near_; }
  void set_near(float near) {
    UpdateProjectionMat();
    near_ = near;
  }

  float far() const { return far_; }
  void set_far(float far) {
    UpdateProjectionMat();
    far_ = far;
  }

  Vec3 right() const {
    return Vec3(view_mat_[0][0], view_mat_[1][0], view_mat_[2][0]);
  }

  Vec3 up() const {
    return Vec3(view_mat_[0][1], view_mat_[1][1], view_mat_[2][1]);
  }

  Vec3 forward() const {
    return Vec3(-view_mat_[0][2], -view_mat_[1][2], -view_mat_[2][2]);
  }

  Vec3 center() const {
    return Vec3(-view_mat_[3][0], -view_mat_[3][1], -view_mat_[3][2]);
  }

  const Mat4& view_mat() const { return view_mat_; }

  const Mat4& projection_mat() const { return projection_mat_; }

  void MoveForward(float d) { set_position(position() + forward() * d); }

  void RotateAround(const Vec3& angle) {
    Vec4 view_mat_3 = view_mat_[3];
    view_mat_ = glm::rotate(view_mat_, angle.x, right());
    view_mat_ = glm::rotate(view_mat_, angle.y, up());
    view_mat_ = glm::rotate(view_mat_, angle.z, -forward());
    view_mat_[3] = view_mat_3;

    Mat4 view_mat_inv = glm::inverse(view_mat_);
    position_ =
        Vec3(view_mat_inv[3][0], view_mat_inv[3][1], view_mat_inv[3][2]);
    glm::extractEulerAngleXYZ(view_mat_, rotation_.x, rotation_.y, rotation_.z);
  }

 private:
  void UpdateViewMat() {
    view_mat_ = glm::eulerAngleXYZ(rotation_.x, rotation_.y, rotation_.z) *
                glm::translate(Mat4(1.f), -position_);
  }

  void UpdateProjectionMat() {
    projection_mat_ = glm::perspective(fovy_, aspect_, near_, far_);
  }

  Vec3 position_ = Vec3(0.f, 0.f, 10.f);
  Vec3 rotation_ = Vec3(0.f, 0.f, 0.f);

  float fovy_ = 90.f / 180.f * PI;
  float aspect_ = 16.f / 9.f;  // widht / hieght
  float near_ = 0.1f;
  float far_ = 100.f;

  Mat4 view_mat_ = Mat4(1.0);
  Mat4 projection_mat_ = Mat4(1.0);
};

}  // namespace glkit

#endif  // GLKIT_GL_CAMERA_HPP_