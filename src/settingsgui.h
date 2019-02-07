#pragma once

#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/vec4.hpp>
#include "imgui.h"

class SettingsGui {
 private:
  GLFWwindow*      _window;
  ImGuiWindowFlags _windowFlags = 0;

  double _lastTime               = 0.0;
  double _printFrameTimeDuration = 0.0;
  double _printDuration          = 0.0;

  int       _particlePerDimension;
  float     _gravity;
  float     _particleSize;
  float     _force;
  glm::vec4 _particleColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

  void setDefault();

 public:
  SettingsGui(GLFWwindow* window);
  ~SettingsGui();

  void setup();
  void render();
  void shutdown();

  int particleCount() const {
    return (int)pow(_particlePerDimension, 3);
  }
  float gravity() const {
    return _gravity;
  }
  float particleSize() const {
    return _particleSize;
  }
  glm::vec4 particleColor() const {
    return _particleColor;
  }
  float force() const {
    return _force;
  }
};
