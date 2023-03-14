#include <algorithm>

#include "glkit/gl_camera.hpp"
#include "glkit/gl_shader.hpp"
#include "glkit/imgui_app.hpp"

namespace glkit {

class GLKitApp : public ImGuiApp {
 public:
  int Init(int width = 1280, int height = 720) {
    ImGuiApp::Init(width, height, "GLKit");

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
        -0.5f, -0.5f, 0.0f,  // 左下
        -0.5f, 0.5f,  0.0f,  // 左上
        0.5f,  -0.5f, 0.0f,  // 右下
        0.5f,  0.5f,  0.0f,  // 右上
    };
    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    return 0;
  }

  int Render() override {
    RenderDemo();
    UiAddCamera();
    ImGui::Render();

    glClearColor(clear_color_.x, clear_color_.y, clear_color_.z,
                 clear_color_.w);
    glClear(GL_COLOR_BUFFER_BIT);
    shader_.Use();
    shader_.SetMat4("mvp", camera_.projection_mat() * camera_.view_mat());
    glBindVertexArray(vao_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    return 0;
  }

 private:
  void UiAddCamera() {
    ImGui::Begin("Camera");
    ImGui::GetFont()->Scale *= 1.2f;
    ImGui::PushFont(ImGui::GetFont());

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
      ImGui::Text("Front(-Z): %8.3f %8.3f %8.3f", f.x, f.y, f.z);
      ImGui::Text("Right(+X): %8.3f %8.3f %8.3f", r.x, r.y, r.z);
      ImGui::Text("   Up(+Y): %8.3f %8.3f %8.3f", u.x, u.y, u.z);
      ImGui::TreePop();
    }

    if (ImGui::TreeNode("View Matrix")) {
      const Mat4& view_mat = camera_.view_mat();
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
          ImGui::Text("%8.3f", view_mat[j][i]);
          ImGui::SameLine();
        }
        ImGui::Text("%8.3f", view_mat[3][i]);
      }
      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Projection Matrix")) {
      const Mat4& projection_mat = camera_.projection_mat();
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
          ImGui::Text("%8.3f", projection_mat[j][i]);
          ImGui::SameLine();
        }
        ImGui::Text("%8.3f", projection_mat[3][i]);
      }
      ImGui::TreePop();
    }

    ImGui::GetFont()->Scale /= 1.2f;
    ImGui::PopFont();

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

  Camera camera_;
  Shader shader_;
  GLuint vbo_;
  GLuint vao_;
};

}  // namespace glkit

int main() {
  glkit::GLKitApp app;
  app.Init();
  app.Run();
  app.Destory();
  return 0;
}