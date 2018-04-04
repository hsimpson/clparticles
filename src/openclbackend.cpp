#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif

#include "openclbackend.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <cmath>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

OpenCLBackend::OpenCLBackend(GLFWwindow* window, SettingsGui* gui, const glm::vec4& boundingBox)
    : _window(window), _gui(gui), _boundingBox(boundingBox) {
}

OpenCLBackend::~OpenCLBackend() {
  glDeleteVertexArrays(1, &_vao);
  glDeleteBuffers(1, &_vboPos);
  glDeleteBuffers(1, &_vboVel);
}

void OpenCLBackend::init(Program* glprogram) {
  _program = glprogram;

  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  auto platform = platforms.front();

#ifdef _WIN32
  cl_context_properties contextProps[] = {
      CL_GL_CONTEXT_KHR,
      (cl_context_properties)glfwGetWGLContext(_window),
      CL_WGL_HDC_KHR,
      (cl_context_properties)GetDC(glfwGetWin32Window(_window)),
      CL_CONTEXT_PLATFORM,
      (cl_context_properties)platform(),
      0};
#endif

  std::vector<cl::Device> devices;
  platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

  _device  = devices.front();
  _context = cl::Context(_device, contextProps);
  _queue   = cl::CommandQueue(_context, _device, CL_QUEUE_PROFILING_ENABLE);  // with profiling enabled

  std::cout << "OpenCL Info:" << std::endl;
  std::cout << "Name: " << _device.getInfo<CL_DEVICE_NAME>() << std::endl;
  std::cout << "Vendor: " << _device.getInfo<CL_DEVICE_VENDOR>() << std::endl;
  std::cout << "Driver Version: " << _device.getInfo<CL_DRIVER_VERSION>() << std::endl;
  std::cout << "Device Profile: " << _device.getInfo<CL_DEVICE_PROFILE>() << std::endl;
  std::cout << "Device version: " << _device.getInfo<CL_DEVICE_VERSION>() << std::endl;
  std::cout << "Max Work Group Size: " << _device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << std::endl;
  std::cout << "Device extensions: " << _device.getInfo<CL_DEVICE_EXTENSIONS>() << std::endl;

  const std::string filename = "./cl_kernels/particle_kernel.cl";
  std::ifstream     infile(filename.c_str());
  std::stringstream buffer;

  buffer << infile.rdbuf();
  std::string srcString = buffer.str();

  // create the program
  cl::Program::Sources sources(1, std::make_pair(srcString.c_str(), srcString.length() + 1));
  cl::Program          clprogram(_context, sources);
  clprogram.build({_device});

  // create the kernel
  _kernel = cl::Kernel(clprogram, "particle_kernel");

  glGenVertexArrays(1, &_vao);

  // create the vbo's
  glGenBuffers(1, &_vboPos);
  glGenBuffers(1, &_vboVel);

  resizeBuffers();

  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _vboPos);

  /*
  _clglPos = cl::BufferGL(_context, CL_MEM_READ_WRITE, _vboPos);
  _kernel.setArg(OpenCLBackend::Positions, _clglPos);

  //glBindBuffer(GL_ARRAY_BUFFER, _vboVel);

  _clglVel = cl::BufferGL(_context, CL_MEM_READ_WRITE, _vboVel);
  _kernel.setArg(OpenCLBackend::Velocities, _clglVel);
  */

  _kernel.setArg(OpenCLBackend::BoundingBox, sizeof(cl_float4), glm::value_ptr(_boundingBox));

  // set the vertex attribute pointers
  // vertex Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

  glBindVertexArray(0);
}

/* only for cpu slow impl */
/*
void OpenCLBackend::initGLBuffers() {
  // calc the cubic root of particle count
  float n = std::cbrt((float)_particleCount);

  float dx = _boundingBox.x / (n + 1.0f);
  float dy = _boundingBox.y / (n + 1.0f);
  float dz = _boundingBox.z / (n + 1.0f);

  for (int i = 0; i < _particleCount; ++i) {
    float x = i % (int)n * dx + dx - (_boundingBox.x / 2);
    float y = ((int)(i / n)) % (int)n * dy + dy - (_boundingBox.y / 2);
    //float y = 0.0f;
    float z = ((int)(i / pow(n, 2))) % (int)n * dz + dz - (_boundingBox.z / 2);

    //Semi-random velocities

    float vx = cos((float)(1234 * i));
    float vy = sin((float)(1234 * i));
    float vz = cos((float)(1234 * i));

    _glPosArray[i] = glm::vec4(x, y, z, 1.0f);
    _glVelArray[i] = glm::vec4(vx, vy, vz, 1.0f);
  }

  glBindBuffer(GL_ARRAY_BUFFER, _vboPos);
  glBufferSubData(GL_ARRAY_BUFFER, 0, _particleCount * 4 * sizeof(float), &_glPosArray[0]);

  glBindBuffer(GL_ARRAY_BUFFER, _vboVel);
  glBufferSubData(GL_ARRAY_BUFFER, 0, _particleCount * 4 * sizeof(float), &_glVelArray[0]);
}
*/

/* only for cpu slow impl*/
/*
void OpenCLBackend::updateGLBuffers(float deltaTime, bool forceActive, const glm::vec4& forcePosition) {
  float g        = _gui->gravity() * deltaTime;
  float bbx_half = _boundingBox.x / 2;
  float bby_half = _boundingBox.y / 2;
  float bbz_half = _boundingBox.z / 2;
  float force    = _gui->force();

  for (int i = 0; i < _particleCount; ++i) {
    //Update Particle positions
    glm::vec4& p = _glPosArray[i];
    glm::vec4& v = _glVelArray[i];

    // update particle pos
    p.x += v.x * deltaTime;
    p.y += v.y * deltaTime;
    p.z += v.z * deltaTime;

    //Add gravity
    v.y -= g;

    //Edge Collision Detection
    if (p.x < -bbx_half) {  //LEFT
      p.x = -2 * bbx_half - p.x;
      v.x *= -0.9f;
    } else if (p.x > bbx_half) {  //RIGHT
      p.x = 2 * bbx_half - p.x;
      v.x *= -0.9f;
    }

    if (p.y < -bby_half) {  //BOTTOM
      p.y = -2 * bby_half - p.y;
      v.y *= -0.45f;  // if its on the bottom we extra dampen
      v.x *= 0.9f;
    } else if (p.y > bby_half) {  //TOP
      p.y = 2 * bby_half - p.y;
      v.y *= -0.9f;
    }

    if (p.z < -bbz_half) {  // FRONT
      p.z = -2 * bbz_half - p.z;
      v.z *= -0.9f;
    } else if (p.z > bbz_half) {  // BACK
      p.z = 2 * bbz_half - p.z;
      v.z *= -0.9f;
    }

    if (forceActive) {
      float dx = forcePosition.x - p.x;
      float dy = forcePosition.y - p.y;
      float dz = forcePosition.z - p.z;

      float dist = sqrt((dx * dx + dy * dy + dz * dz));
      if (dist < 1)
        dist = 1;                       //This line prevents anything that is really close from getting a huge force
      v.y += dy / dist * force / dist;  //Use linear falloff
      v.x += dx / dist * force / dist;  //Exponential gives sort of a ball where the mouse is and thats it
      v.z += dz / dist * force / dist;
    }
  }
  glBindBuffer(GL_ARRAY_BUFFER, _vboPos);
  glBufferSubData(GL_ARRAY_BUFFER, 0, _particleCount * 4 * sizeof(float), &_glPosArray[0]);

  glBindBuffer(GL_ARRAY_BUFFER, _vboVel);
  glBufferSubData(GL_ARRAY_BUFFER, 0, _particleCount * 4 * sizeof(float), &_glVelArray[0]);
}
*/

void OpenCLBackend::runKernel(OpenCLBackend::RunMode runmode, bool forceActive, const glm::vec4& forcePosition) {
  double currentTime = glfwGetTime();
  float  duration    = float(currentTime - _lastTime);
  _lastTime          = currentTime;
  if (resizeBuffers()) {
    runmode = OpenCLBackend::Init;
  }

  _particleColor.a = _gui->particleOpacity();

  // ensure OpenGL is finished
  glFinish();
  _kernel.setArg(OpenCLBackend::Fuction, (int)runmode);
  _kernel.setArg(OpenCLBackend::ParticleCount, _particleCount);
  _kernel.setArg(OpenCLBackend::DeltaTime, duration);
  _kernel.setArg(OpenCLBackend::Gravity, _gui->gravity());
  _kernel.setArg(OpenCLBackend::ForceActive, forceActive ? 1 : 0);
  _kernel.setArg(OpenCLBackend::Force, _gui->force());
  _kernel.setArg(OpenCLBackend::ForcePos, sizeof(cl_float4), glm::value_ptr(forcePosition));

  std::vector<cl::Memory> shared_objects = {_clglPos, _clglVel};
  _queue.enqueueAcquireGLObjects(&shared_objects);
  _queue.finish();

  _queue.enqueueNDRangeKernel(_kernel, cl::NDRange(0), cl::NDRange(_particleCount));
  _queue.finish();

  _queue.enqueueReleaseGLObjects(&shared_objects);
  _queue.finish();

  //updateGLBuffers(duration, forceActive, forcePosition);
}

void OpenCLBackend::render() {
  _program->setUniformValue("u_color", _particleColor);
  glPointSize(_gui->particleSize());
  glBindVertexArray(_vao);
  glDrawArrays(GL_POINTS, 0, _particleCount);
  glBindVertexArray(0);
}

bool OpenCLBackend::resizeBuffers() {
  int newParticleCount = _gui->particleCount();
  if (newParticleCount != _particleCount) {
    _particleCount = newParticleCount;

    //_glPosArray.resize(_particleCount);
    //_glVelArray.resize(_particleCount);

    glBindBuffer(GL_ARRAY_BUFFER, _vboPos);
    glBufferData(GL_ARRAY_BUFFER, _particleCount * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    _clglPos = cl::BufferGL(_context, CL_MEM_READ_WRITE, _vboPos);
    _kernel.setArg(OpenCLBackend::Positions, _clglPos);

    glBindBuffer(GL_ARRAY_BUFFER, _vboVel);
    glBufferData(GL_ARRAY_BUFFER, _particleCount * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    _clglVel = cl::BufferGL(_context, CL_MEM_READ_WRITE, _vboVel);
    _kernel.setArg(OpenCLBackend::Velocities, _clglVel);

    //glBindBuffer(GL_ARRAY_BUFFER, _vboVel);

    //initGLBuffers();
    return true;
  }
  return false;
}
