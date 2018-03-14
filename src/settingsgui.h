#pragma once

#include <GLFW/glfw3.h>
#include "imgui.h"

class SettingsGui {
 private:
  GLFWwindow*      _window;
  ImGuiWindowFlags _windowFlags = 0;

  double _lastTime               = 0.0;
  double _printFrameTimeDuration = 0.0;
  double _printDuration          = 0.0;

  int   _particleCount;
  float _gravity;
  float _particleSize;
  float _particleOpacity;
  float _force;

  void setDefault();

 public:
  SettingsGui(GLFWwindow* window);
  ~SettingsGui();

  void setup();
  void render();
  void shutdown();
};
