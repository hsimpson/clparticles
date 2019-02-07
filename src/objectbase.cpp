#include "objectbase.h"
#include <cstddef>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

ObjectBase::ObjectBase(GLenum drawMode)
    : _drawMode(drawMode) {
}

ObjectBase::~ObjectBase() {
  glDeleteVertexArrays(1, &_vao);
  glDeleteBuffers(1, &_vbo);
  glDeleteBuffers(1, &_ebo);
}

void ObjectBase::init(Program* program) {
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glGenBuffers(1, &_vbo);
  glGenBuffers(1, &_ebo);

  _program = program;

  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLushort), &_indices[0], GL_STATIC_DRAW);

  // set the vertex attribute pointers
  // vertex Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // vertex Colors
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

  glBindVertexArray(0);
}

void ObjectBase::updated_model_matrix() {
  glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);
  glm::mat4 rotationMatrix    = glm::toMat4(_rotation);
  glm::mat4 scaleMatrix       = glm::scale(glm::mat4(1.0f), _scale);
  _modelMatrix                = translationMatrix * rotationMatrix * scaleMatrix;
}

const glm::vec3& ObjectBase::translate(const glm::vec3& translate) {
  _position += translate;
  updated_model_matrix();
  return _position;
}

void ObjectBase::rotate(const glm::quat& rotate) {
  _rotation = rotate * _rotation;
  updated_model_matrix();
}

void ObjectBase::render() {
  _program->setUniformValue("modelMatrix", _modelMatrix);
  glBindVertexArray(_vao);
  glDrawElements(_drawMode, (GLsizei)_indices.size(), GL_UNSIGNED_SHORT, 0);
  glBindVertexArray(0);
}
