#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "program.h"

class ObjectBase {
 protected:
  GLuint      _VAO;
  GLuint      _VBO;
  GLuint      _EBO;
  GLenum      _drawMode;
  bool        _initialized = false;
  Program*    _program;
  glm::mat4x4 _modelMatrix;
  glm::quat   _rotation;
  glm::vec3   _position;
  glm::vec3   _scale = glm::vec3(1.0, 1.0, 1.0);

  struct Vertex {
    // position
    glm::vec3 pos;
    // vertex color
    glm::vec4 col;

    Vertex(const glm::vec3& pos, const glm::vec4& col) : pos(pos), col(col){};
  };

  std::vector<Vertex>   _vertices;
  std::vector<GLushort> _indices;

 private:
  void updated_model_matrix();

 public:
  ObjectBase(GLenum drawMode = GL_TRIANGLES);
  virtual ~ObjectBase();
  void init(Program* program);
  void translate(const glm::vec3& translate);
  void rotate(const glm::quat& rotate);
  void render();
};