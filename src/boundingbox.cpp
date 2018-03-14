#include "boundingbox.h"

BoundingBox::BoundingBox(const glm::vec3& dimensions) : ObjectBase(GL_LINES), _dimensions(dimensions) {
  float halfx = _dimensions.x / 2;
  float halfy = _dimensions.y / 2;
  float halfz = _dimensions.z / 2;

  /* the cube:


     v5-----------v6
    / |          / |
   /  |         /  |
  v2----------v1   |
  |   |        |   |
  |   |        |   |
  |  v4--------|--v7
  | /          |  /
  |/           | /
  v3-----------v0

  */
  glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

  _vertices = {// front vertices:
               ObjectBase::Vertex(glm::vec3(halfx, -halfy, -halfz), color),
               ObjectBase::Vertex(glm::vec3(halfx, halfy, -halfz), color),
               ObjectBase::Vertex(glm::vec3(-halfx, halfy, -halfz), color),
               ObjectBase::Vertex(glm::vec3(-halfx, -halfy, -halfz), color),

               // back vertices:
               ObjectBase::Vertex(glm::vec3(-halfx, -halfy, halfz), color),
               ObjectBase::Vertex(glm::vec3(-halfx, halfy, halfz), color),
               ObjectBase::Vertex(glm::vec3(halfx, halfy, halfz), color),
               ObjectBase::Vertex(glm::vec3(halfx, -halfy, halfz), color)};

  // create the indices for the lines
  _indices = {// front
              0, 1, 1, 2, 2, 3, 3, 0,

              // left
              3, 2, 2, 5, 5, 4, 4, 3,

              // right
              7, 6, 6, 1, 1, 0, 0, 7,

              // back
              4, 5, 5, 6, 6, 7, 7, 4};
}

BoundingBox::~BoundingBox() {
}
