#pragma once
#include <GL/gl3w.h>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <glm/vec4.hpp>
#include "program.h"
#include "settingsgui.h"

class OpenCLBackend {
 private:
  GLFWwindow*      _window;
  SettingsGui*     _gui;
  cl::Device       _device;
  cl::Context      _context;
  cl::Kernel       _kernel;
  cl::BufferGL     _clglPos;
  cl::BufferGL     _clglVel;
  cl::CommandQueue _queue;
  glm::vec4        _boundingBox;
  glm::vec4        _particleColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

  std::vector<glm::vec4> _glPosArray;
  std::vector<glm::vec4> _glVelArray;
  Program*               _program = nullptr;

  GLuint _vao;
  GLuint _vboPos;
  GLuint _vboVel;

  double _lastTime      = 0;
  int    _particleCount = 0;

  enum KernelParam {
    Positions,
    Velocities,
    Fuction,
    ParticleCount,
    DeltaTime,
    BoundingBox,
    Gravity,
    ForceActive,
    Force,
    ForcePos
  };

  void resizeBuffers();
  void initGLBuffers();
  void updateGLBuffers(float deltaTime, bool forceActive, const glm::vec4& forcePosition);

 public:
  enum RunMode { Init,
                 Update,
                 Resize };
  OpenCLBackend(GLFWwindow* window, SettingsGui* gui, const glm::vec4& boundingBox);
  virtual ~OpenCLBackend();

  void init(Program* program);

  void runKernel(OpenCLBackend::RunMode runmode, bool forceActive, const glm::vec4& forcePosition);
  void render();
};
