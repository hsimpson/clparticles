#include "triangle.h"

Triangle::Triangle() : ObjectBase(GL_TRIANGLES) {
  _vertices = {
      ObjectBase::Vertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),   // top
      ObjectBase::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),  // right
      ObjectBase::Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))  // left
  };

  _indices = {0, 1, 2};
}

Triangle::~Triangle() {
}
