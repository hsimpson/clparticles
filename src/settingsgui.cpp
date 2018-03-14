#include "settingsgui.h"
#include "imgui_impl_glfw_gl3.h"

SettingsGui::SettingsGui(GLFWwindow* window) : _window(window) {
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGui_ImplGlfwGL3_Init(_window, false);
  ImGui::StyleColorsDark();

  _windowFlags = ImGuiWindowFlags_NoMove;
  _windowFlags |= ImGuiWindowFlags_NoResize;
  _windowFlags |= ImGuiWindowFlags_NoSavedSettings;

  _lastTime = glfwGetTime();
  setDefault();
}

SettingsGui::~SettingsGui() {
  ImGui_ImplGlfwGL3_Shutdown();
  ImGui::DestroyContext();
}

void SettingsGui::setDefault() {
  _particleCount   = 1000000;
  _gravity         = 0.98f;
  _particleSize    = 1.0f;
  _particleOpacity = 0.5f;
  _force           = 0.1f;
}

void SettingsGui::setup() {
}

void SettingsGui::render() {
  // Measure speed
  /*
  double currentTime = glfwGetTime();
  double duration    = currentTime - _lastTime;
  _lastTime          = currentTime;
  _printFrameTimeDuration += duration;
  if (_printFrameTimeDuration >= 1.0) {
    _printDuration = duration;
  }
  */
  ImGui_ImplGlfwGL3_NewFrame();

  /**/
  int width, height;
  glfwGetWindowSize(_window, &width, &height);
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(400.0, (float)height));
  ImGui::Begin("Settings", NULL, _windowFlags);
  // ImGui::Text("Frame time: %.3f ms (%.1f FPS)", _printDuration, 1000.0 / _printDuration);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
              ImGui::GetIO().Framerate);
  ImGui::SliderInt("Particle count", &_particleCount, 1000, 100000000, "%.0f");
  ImGui::SliderFloat("Gravity", &_gravity, -1.0f, 1.0f, "%.2f");
  ImGui::SliderFloat("Particle size", &_particleSize, 1.0f, 10.0f, "%.2f");
  ImGui::SliderFloat("Particle opacity", &_particleOpacity, 0.0f, 1.0f, "%.2f");
  ImGui::SliderFloat("Force", &_force, -5.0f, 5.0f, "%.2f");
  if (ImGui::Button("Set default values")) {
    setDefault();
  }
  ImGui::Separator();
  ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Controls:");
  float spacing = 160;

  ImGui::Text("Left Mouse Button:");
  ImGui::SameLine(spacing);
  ImGui::Text("Scene rotation");

  ImGui::Text("Mouse Wheel:");
  ImGui::SameLine(spacing);
  ImGui::Text("Zoom in/out");

  ImGui::Text("Space:");
  ImGui::SameLine(spacing);
  ImGui::Text("Activate force");

  ImGui::Text("W:");
  ImGui::SameLine(spacing);
  ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "Move force -Z");

  ImGui::Text("A:");
  ImGui::SameLine(spacing);
  ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Move force -X");

  ImGui::Text("S:");
  ImGui::SameLine(spacing);
  ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "Move force +Z");

  ImGui::Text("D:");
  ImGui::SameLine(spacing);
  ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Move force +X");

  ImGui::Text("Page Up:");
  ImGui::SameLine(spacing);
  ImGui::TextColored(ImVec4(.0f, 1.0f, 0.0f, 1.0f), "Move force +Y");

  ImGui::Text("Page Down:");
  ImGui::SameLine(spacing);
  ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Move force -Y");

  ImGui::End();
  /**/

  // demo window from ImGui
  // ImGui::ShowDemoWindow();

  ImGui::Render();
  ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void SettingsGui::shutdown() {
}
