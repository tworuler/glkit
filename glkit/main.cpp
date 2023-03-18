#include <algorithm>

#include "glkit/gl_camera.hpp"
#include "glkit/gl_mesh.hpp"
#include "glkit/gl_mesh_manager.hpp"
#include "glkit/gl_model.hpp"
#include "glkit/gl_shader.hpp"
#include "glkit/gl_shader_manager.hpp"
#include "glkit/gl_square.hpp"
#include "glkit/gl_xy_plane.hpp"
#include "glkit/imgui_app.hpp"

namespace glkit {

class GLKitApp : public ImGuiApp {
 public:
  int Init(int width = 1280, int height = 720,
           const char* name = "GLKit") override {
    ImGuiApp::Init(width, height, name);
    clear_color_ = ImVec4(0.23, 0.23, 0.23, 1);

    auto cube_mesh =
        mesh_manager_.AddMeshFromObjFile("cube", "objects/cube.obj");
    auto sphere_mesh =
        mesh_manager_.AddMeshFromObjFile("sphere", "objects/sphere.obj");
    auto monkey_mesh =
        mesh_manager_.AddMeshFromObjFile("monkey", "objects/monkey.obj");

    auto xy_plane_shader = shader_manager_.AddShaderFromFile(
        "xy_plane", "shaders/xy_plane.vs", "shaders/xy_plane.fs");
    auto light_shader = shader_manager_.AddShaderFromFile(
        "light", "shaders/light.vs", "shaders/light.fs");
    auto mesh_shader = shader_manager_.AddShaderFromFile(
        "mesh", "shaders/mesh.vs", "shaders/mesh.fs");

    square_.Init();
    xy_plane_.Init(xy_plane_shader, 100);
    light_.Init(sphere_mesh, light_shader, true);
    cube_.Init(cube_mesh, mesh_shader);
    sphere_.Init(sphere_mesh, mesh_shader);
    monkey_.Init(monkey_mesh, mesh_shader);

    glEnable(GL_DEPTH_TEST);

    return 0;
  }

  int Render() override {
    RenderUi();
    ImGui::Render();

    glClearColor(clear_color_.x, clear_color_.y, clear_color_.z,
                 clear_color_.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (show_xy_plane_)
      xy_plane_.Draw(camera_.projection_mat() * camera_.view_mat());
    if (show_light_) light_.Draw(camera_.view_mat(), camera_.projection_mat());
    if (show_square_)
      square_.Draw(camera_.projection_mat() * camera_.view_mat());
    if (show_cube_) {
      cube_.SetLight(light_.position(), light_.color());
      cube_.Draw(camera_.view_mat(), camera_.projection_mat());
    }
    if (show_sphere_) {
      sphere_.SetLight(light_.position(), light_.color());
      sphere_.Draw(camera_.view_mat(), camera_.projection_mat());
    }
    if (show_monkey_) {
      monkey_.SetLight(light_.position(), light_.color());
      monkey_.Draw(camera_.view_mat(), camera_.projection_mat());
    }

    return 0;
  }

 private:
  void RenderUi() {
    ImGui::Begin("GLKit");
    ImGui::Checkbox("ImGui Demo Window", &show_demo_window_);
    ImGui::Text("average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::ColorEdit3("Clear Color", (float*)&clear_color_);
    ImGui::Checkbox("Show XY Plane", &show_xy_plane_);
    ImGui::Checkbox("Show Camera", &show_camera_);
    ImGui::Checkbox("Show Light", &show_light_);
    ImGui::Checkbox("Show Cube", &show_cube_);
    ImGui::Checkbox("Show Sphere", &show_sphere_);
    ImGui::Checkbox("Show Square", &show_square_);
    ImGui::Checkbox("Show Monkey", &show_monkey_);
    ImGui::End();

    if (show_demo_window_) ImGui::ShowDemoWindow(&show_demo_window_);
    if (show_camera_) UiAddCamera();
    if (show_light_) UiAddModel("Light", &light_);
    if (show_cube_) UiAddModel("Cube", &cube_);
    if (show_sphere_) UiAddModel("Sphere", &sphere_);
    if (show_monkey_) UiAddModel("Monkey", &monkey_);
  }

  void UiAddCamera() {
    ImGui::Begin("Camera");

    Vec3 position = camera_.position();
    ImGui::InputFloat("PositionX", &position.x, 1.f, 10.f, "%.1f");
    ImGui::InputFloat("PositionY", &position.y, 1.f, 10.f, "%.1f");
    ImGui::InputFloat("PositionZ", &position.z, 1.f, 10.f, "%.1f");
    camera_.set_position(position);

    float pitch = camera_.pitch() / PI * 180.f;
    ImGui::InputFloat("Pitch(X)", &pitch, 1.f, 10.f, "%.1f");
    pitch = std::max(std::min(pitch, 90.f), -90.f);
    camera_.set_pitch(pitch / 180.f * PI);
    float yaw = camera_.yaw() / PI * 180.f;
    ImGui::InputFloat("Yaw(Y)", &yaw, 1.f, 10.f, "%.1f");
    if (yaw < -180.f) yaw += 360.f;
    if (yaw > 180.f) yaw -= 360.f;
    camera_.set_yaw(yaw / 180.f * PI);

    float fovy = camera_.fovy() / PI * 180.f;
    ImGui::InputFloat("FovY", &fovy, 1.f, 10.f, "%.1f");
    camera_.set_fovy(fovy / 180.f * PI);
    static int aspect[2] = {16, 9};
    ImGui::InputInt2("Aspect(W/H)", aspect);
    camera_.set_aspect(static_cast<float>(aspect[0]) / aspect[1]);
    float near = camera_.near();
    ImGui::InputFloat("Near", &near, 1.f, 10.f, "%.1f");
    camera_.set_near(near);
    float far = camera_.far();
    ImGui::InputFloat("Far", &far, 1.f, 10.f, "%.1f");
    camera_.set_far(far);

    if (ImGui::TreeNode("Direction")) {
      const Vec3& f = camera_.front();
      const Vec3& r = camera_.right();
      const Vec3& u = camera_.up();
      ImGui::Text("Front(-Z): %6.3f %6.3f %6.3f", f.x, f.y, f.z);
      ImGui::Text("Right(+X): %6.3f %6.3f %6.3f", r.x, r.y, r.z);
      ImGui::Text("   Up(+Y): %6.3f %6.3f %6.3f", u.x, u.y, u.z);
      ImGui::TreePop();
    }

    if (ImGui::TreeNode("View Matrix")) {
      const Mat4& view_mat = camera_.view_mat();
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
          ImGui::Text("%6.3f", view_mat[j][i]);
          ImGui::SameLine();
        }
        ImGui::Text("%6.3f", view_mat[3][i]);
      }
      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Projection Matrix")) {
      const Mat4& projection_mat = camera_.projection_mat();
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
          ImGui::Text("%6.3f", projection_mat[j][i]);
          ImGui::SameLine();
        }
        ImGui::Text("%6.3f", projection_mat[3][i]);
      }
      ImGui::TreePop();
    }

    ImGui::End();

    const auto& io = ImGui::GetIO();
    if (io.MouseWheel != 0.f) {
      camera_.MoveFront(io.MouseWheel);
    }
    // 图像移动方向与相机移动方向相反
    if (ImGui::IsKeyDown(ImGuiKey_A)) {
      camera_.MoveRight(0.1f);
    }
    if (ImGui::IsKeyDown(ImGuiKey_D)) {
      camera_.MoveRight(-0.1f);
    }
    if (ImGui::IsKeyDown(ImGuiKey_W)) {
      camera_.MoveUp(-0.1f);
    }
    if (ImGui::IsKeyDown(ImGuiKey_S)) {
      camera_.MoveUp(0.1f);
    }
    if (ImGui::IsKeyDown(ImGuiKey_Q)) {
      camera_.TurnRight(1.f / 180.f * PI);
    }
    if (ImGui::IsKeyDown(ImGuiKey_E)) {
      camera_.TurnLeft(1.f / 180.f * PI);
    }
    if (ImGui::IsKeyDown(ImGuiKey_2)) {
      camera_.TurnDown(1.f / 180.f * PI);
    }
    if (ImGui::IsKeyDown(ImGuiKey_X)) {
      camera_.TurnUp(1.f / 180.f * PI);
    }
  }

  void UiAddModel(const char* name, Model* model) {
    ImGui::Begin(name);
    Vec3 color = model->color();
    ImGui::ColorEdit3("Color", &color.x);
    model->set_color(color);
    Vec3 position = model->position();
    ImGui::InputFloat("PX", &position.x, 0.1f, 1.f, "%.1f");
    ImGui::InputFloat("PY", &position.y, 0.1f, 1.f, "%.1f");
    ImGui::InputFloat("PZ", &position.z, 0.1f, 1.f, "%.1f");
    model->set_position(position);
    Vec3 rotation = model->rotation() / PI * 180.f;
    ImGui::InputFloat("RX", &rotation.x, 1.f, 10.f, "%.1f");
    ImGui::InputFloat("RY", &rotation.y, 1.f, 10.f, "%.1f");
    ImGui::InputFloat("RZ", &rotation.z, 1.f, 10.f, "%.1f");
    model->set_rotation(rotation / 180.f * PI);
    Vec3 scale = model->scale();
    ImGui::InputFloat("SX", &scale.x, 0.1f, 1.f, "%.1f");
    ImGui::InputFloat("SY", &scale.y, 0.1f, 1.f, "%.1f");
    ImGui::InputFloat("SZ", &scale.z, 0.1f, 1.f, "%.1f");
    model->set_scale(scale);

    if (ImGui::TreeNode("Model Matrix")) {
      const Mat4& model_mat = model->GetModelMatrix();
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
          ImGui::Text("%6.3f", model_mat[j][i]);
          ImGui::SameLine();
        }
        ImGui::Text("%6.3f", model_mat[3][i]);
      }
      ImGui::TreePop();
    }
    ImGui::End();
  }

  Camera camera_;
  ShaderManager shader_manager_;
  MeshManager mesh_manager_;
  XyPlane xy_plane_;
  Square square_;
  Model light_;
  Model cube_;
  Model sphere_;
  Model monkey_;

  bool show_xy_plane_ = true;
  bool show_camera_ = true;
  bool show_light_ = true;
  bool show_cube_ = true;
  bool show_square_ = false;
  bool show_sphere_ = false;
  bool show_monkey_ = false;
};

}  // namespace glkit

int main() {
  glkit::GLKitApp app;
  app.Init();
  app.Run();
  app.Destory();
  return 0;
}