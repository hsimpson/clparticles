#include "program.h"
#include <GL/gl3w.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Program::Program() {
  _id = glCreateProgram();
}
Program::~Program() {
  while (!_shaders.empty()) {
    detach(*_shaders.begin());
  }
  glDeleteProgram(_id);
}

void Program::attach(Shader* shader) {
  glAttachShader(_id, shader->id());
  _shaders.insert(shader);
}

void Program::detach(Shader* shader) {
  glDetachShader(_id, shader->id());
  _shaders.erase(shader);
}

void Program::link() {
  for (Shader* shader : _shaders) {
    if (shader->isCompiled()) continue;

    shader->compile();
  }

  glLinkProgram(_id);
  _linked = true;
  int  success;
  char infoLog[512];
  glGetProgramiv(_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(_id, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    _linked = false;
  }
}

void Program::use() {
  if (!_linked) {
    link();
  }
  glUseProgram(_id);
}
GLint Program::getLocation(const std::string& name) {
  GLint ret = _uniformLocations[name];
  if (!ret) {
    ret                     = glGetUniformLocation(_id, name.c_str());
    _uniformLocations[name] = ret;
  }
  return ret;
}
void Program::setUniformValue(const std::string& name, const glm::mat4& value) {
  use();
  glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Program::setUniformValue(const std::string& name, const glm::vec4& value) {
  use();
  glUniform4fv(getLocation(name), 1, glm::value_ptr(value));
}
