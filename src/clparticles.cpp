// clparticles.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <GL/gl3w.h>  // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/quaternion.hpp>        // glm::quat
#include <glm/mat4x4.hpp>                // glm::mat4
#include <iostream>
#include <sstream>
#include "boundingbox.h"
#include "settingsgui.h"
#include "triangle.h"

void error_callback(int error, const char* description);
void gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                       const void* userParam);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void initializeGL(GLFWwindow* window);
void toggleFullScreen(GLFWwindow* window);
void updateViewMatrix();

glm::mat4    g_projectionMatrix;
glm::vec3    g_viewTranslation = glm::vec3(0.0f, 0.0f, -10.0f);
glm::quat    g_viewRotation;
glm::mat4    g_viewMatrix;
glm::vec2    g_currentCursorPos;
glm::ivec2   g_currentWindowSize = glm::ivec2(1280, 720);
glm::ivec2   g_currentWindowPos;
bool         g_lmb_pressed = false;
bool         g_fullscreen  = false;
const float  g_near        = 0.1f;
const float  g_far         = 100.0f;
GLFWwindow*  g_window      = nullptr;
SettingsGui* g_settingsGui = nullptr;

Triangle    g_triangle;
BoundingBox g_box(glm::vec3(8.0f, 5.0f, 5.0f));

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
  g_window = glfwCreateWindow(g_currentWindowSize.x, g_currentWindowSize.y, "CLParticles", NULL, NULL);
  if (!g_window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwGetWindowPos(g_window, &g_currentWindowPos.x, &g_currentWindowPos.y);

  // input callbacks
  glfwSetKeyCallback(g_window, key_callback);
  glfwSetMouseButtonCallback(g_window, mouse_button_callback);
  glfwSetScrollCallback(g_window, scroll_callback);
  glfwSetCursorPosCallback(g_window, cursor_position_callback);

  /* Make the window's context current */
  glfwMakeContextCurrent(g_window);
  glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);

  // glfwSwapInterval(1); // Enable vsync
  gl3wInit();

  initializeGL(g_window);

  Program* boxProgram = new Program();
  boxProgram->attach(Shader::fromFile(Shader::Vertex, "./shaders/default.vert"));
  boxProgram->attach(Shader::fromFile(Shader::Fragment, "./shaders/default.frag"));

  g_settingsGui = new SettingsGui(g_window);

  g_triangle.init(boxProgram);
  g_box.init(boxProgram);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(g_window)) {
    glfwPollEvents();

    // render
    // ------
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // update matrices
    boxProgram->setUniformValue("projectionMatrix", g_projectionMatrix);
    boxProgram->setUniformValue("viewMatrix", g_viewMatrix);

    g_triangle.render();
    g_box.render();

    /* render gui*/
    g_settingsGui->render();

    glfwSwapBuffers(g_window);
  }
  g_settingsGui->shutdown();
  glfwTerminate();
  return 0;
}

void error_callback(int error, const char* description) {
  std::cerr << "Error " << error << ": " << description << std::endl;
}

void initializeGL(GLFWwindow* window) {
#if _DEBUG
  if (glDebugMessageCallback) {
    std::cout << "Register OpenGL debug callback " << std::endl;
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_error_callback, nullptr);
    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
  } else
    std::cout << "glDebugMessageCallback not available" << std::endl;
#endif

  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  g_projectionMatrix = glm::perspective(glm::radians(45.0f), (float)display_w / (float)display_h, g_near, g_far);
  glViewport(0, 0, display_w, display_h);
  updateViewMatrix();
}

void gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                       const void* userParam) {
  std::cout << "---------------------opengl-callback-start------------" << std::endl;
  std::cout << "message: " << message << std::endl;
  std::cout << "type: ";
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      std::cout << "ERROR";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      std::cout << "DEPRECATED_BEHAVIOR";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      std::cout << "UNDEFINED_BEHAVIOR";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      std::cout << "PORTABILITY";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      std::cout << "PERFORMANCE";
      break;
    case GL_DEBUG_TYPE_OTHER:
      std::cout << "OTHER";
      break;
  }
  std::cout << std::endl;

  std::cout << "id: " << id << std::endl;
  std::cout << "severity: ";
  switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
      std::cout << "LOW";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      std::cout << "MEDIUM";
      break;
    case GL_DEBUG_SEVERITY_HIGH:
      std::cout << "HIGH";
      break;
  }
  std::cout << std::endl;
  std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (ImGui::GetIO().WantCaptureKeyboard) return;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
  if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
    toggleFullScreen(window);
  }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  if (ImGui::GetIO().WantCaptureMouse) return;
  g_viewTranslation.z += (float)yoffset * 0.5f;
  updateViewMatrix();
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (ImGui::GetIO().WantCaptureMouse) return;

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    g_lmb_pressed = true;
    std::cout << "LMB pressed" << std::endl;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    g_currentCursorPos.x = (float)xpos;
    g_currentCursorPos.y = (float)ypos;
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    g_lmb_pressed = false;
    std::cout << "LMB released" << std::endl;
  }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
  if (ImGui::GetIO().WantCaptureMouse) return;
  if (g_lmb_pressed) {
    // std::cout << "pos: " << xpos << ", " << ypos << std::endl;
    glm::vec2 currentPos(xpos, ypos);
    glm::vec2 offset = (currentPos - g_currentCursorPos) * 0.5f;

    g_viewRotation = glm::angleAxis(glm::radians(offset.x), glm::vec3(0.0f, 1.0f, 0.0f)) * g_viewRotation;
    g_viewRotation = glm::angleAxis(glm::radians(offset.y), glm::vec3(1.0f, 0.0f, 0.0f)) * g_viewRotation;

    updateViewMatrix();
    g_currentCursorPos = currentPos;
  }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  g_projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, g_near, g_far);
  glViewport(0, 0, width, height);
}

void toggleFullScreen(GLFWwindow* window) {
  g_fullscreen = !g_fullscreen;
  if (g_fullscreen) {
    // backup pos and size
    glfwGetWindowPos(g_window, &g_currentWindowPos.x, &g_currentWindowPos.y);
    glfwGetWindowSize(g_window, &g_currentWindowSize.x, &g_currentWindowSize.y);

    // get reolution of monitor
    GLFWmonitor*       monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode    = glfwGetVideoMode(monitor);

    // switch to full screen

    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
  } else {
    glfwSetWindowMonitor(window, nullptr, g_currentWindowPos.x, g_currentWindowPos.y, g_currentWindowSize.x,
                         g_currentWindowSize.y, GLFW_DONT_CARE);
  }
}

void updateViewMatrix() {
  glm::mat4 translationMatrix = glm::translate(glm::mat4x4(), g_viewTranslation);
  glm::mat4 rotationMatrix    = glm::toMat4(g_viewRotation);
  g_viewMatrix                = translationMatrix * rotationMatrix;
}
