#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const Shader::Type type, const std::string& src) : _src(src) {
  _id = glCreateShader(type);
}

Shader::~Shader() {
  glDeleteShader(_id);
}

Shader* Shader::fromFile(const Shader::Type type, const std::string& filename) {
  Shader*           p = new Shader(type);
  std::ifstream     infile(filename.c_str());
  std::stringstream buffer;

  buffer << infile.rdbuf();
  p->_src = buffer.str();

  return p;
}

bool Shader::compile() {
  bool ret = true;

  const char* c_str = _src.c_str();
  glShaderSource(_id, 1, &c_str, NULL);
  glCompileShader(_id);
  int  success;
  char infoLog[512];
  glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    ret = false;
    glGetShaderInfoLog(_id, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  _compiled = ret;
  return ret;
}
