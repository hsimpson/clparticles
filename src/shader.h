#pragma once
#include <GL/gl3w.h>
#include <string>

class Shader {
 public:
  enum Type { Vertex = GL_VERTEX_SHADER, Fragment = GL_FRAGMENT_SHADER };

 private:
  GLuint _id;
  // Shader::Type _type;
  std::string _src;
  bool        _compiled = false;

 public:
  Shader(const Shader::Type type, const std::string& src = "");
  ~Shader();

  static Shader* fromFile(const Shader::Type type, const std::string& filename);
  GLuint         id() const {
    return _id;
  }
  bool isCompiled() const {
    return _compiled;
  }

  bool compile();
};
