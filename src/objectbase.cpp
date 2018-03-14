#include "objectbase.h"
#include <cstddef>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

ObjectBase::ObjectBase(GLenum drawMode) : _drawMode(drawMode) {
}

ObjectBase::~ObjectBase() {
  glDeleteVertexArrays(1, &_VAO);
  glDeleteBuffers(1, &_VBO);
  glDeleteBuffers(1, &_EBO);
}

void ObjectBase::init(Program* program) {
  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &_VBO);
  glGenBuffers(1, &_EBO);

  _program = program;
  glBindVertexArray(_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
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
  glm::mat4 translationMatrix = glm::translate(glm::mat4x4(), _position);
  glm::mat4 rotationMatrix    = glm::toMat4(_rotation);
  glm::mat4 scaleMatrix       = glm::scale(glm::mat4x4(), _scale);
  _modelMatrix                = translationMatrix * rotationMatrix * scaleMatrix;
}

void ObjectBase::translate(const glm::vec3& translate) {
  _position += translate;
  updated_model_matrix();
}

void ObjectBase::rotate(const glm::quat& rotate) {
  _rotation = rotate * _rotation;
  updated_model_matrix();
}

void ObjectBase::render() {
  _program->setUniformValue("modelMatrix", _modelMatrix);
  glBindVertexArray(_VAO);
  glDrawElements(_drawMode, (GLsizei)_indices.size(), GL_UNSIGNED_SHORT, 0);
  glBindVertexArray(0);
}
