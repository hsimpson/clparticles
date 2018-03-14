#pragma once
#include <glm/mat4x4.hpp>  // glm::mat4
#include <map>
#include <set>
#include "shader.h"

class Program {
 private:
  int                          _id;
  std::set<Shader*>            _shaders;
  bool                         _linked = false;
  std::map<std::string, GLint> _uniformLocations;

 public:
  Program();
  ~Program();

  void attach(Shader* shader);
  template <class... Shaders>
  void attach(Shader* shader, Shaders... shaders);
  void detach(Shader* shader);

  void link();
  void use();

  GLint getLocation(const std::string& name);
  void  setUniformValue(const std::string& name, const glm::mat4& value);
};
