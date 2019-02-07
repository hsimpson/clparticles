#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "program.h"

class ObjectBase {
 protected:
  GLuint      _vao;
  GLuint      _vbo;
  GLuint      _ebo;
  GLenum      _drawMode;
  bool        _initialized = false;
  Program*    _program     = nullptr;
  glm::mat4   _modelMatrix = glm::mat4(1.0f);
  glm::quat   _rotation    = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3   _position    = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3   _scale       = glm::vec3(1.0f, 1.0f, 1.0f);

  struct Vertex {
    // position
    glm::vec3 pos;
    // vertex color
    glm::vec4 col;

    Vertex(const glm::vec3& pos, const glm::vec4& col)
        : pos(pos), col(col){};
  };

  std::vector<Vertex>   _vertices;
  std::vector<GLushort> _indices;

  void updated_model_matrix();

 public:
  ObjectBase(GLenum drawMode = GL_TRIANGLES);
  virtual ~ObjectBase();
  void                     init(Program* program);
  virtual const glm::vec3& translate(const glm::vec3& translate);
  void                     rotate(const glm::quat& rotate);
  virtual void             render();
};
