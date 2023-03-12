#include "glkit/imgui_app.hpp"

int main() {
  glkit::ImGuiApp app;
  app.Init();
  app.Run();
  app.Destory();
  return 0;
}