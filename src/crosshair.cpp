#include "crosshair.h"
#include <iostream>

CrossHair::CrossHair(const glm::vec3& dimensions, const glm::vec3& bounding)
    : ObjectBase(GL_LINES), _dimensions(dimensions) {
  _halfBounding  = bounding * 0.5f;
  _nhalfBounding = _halfBounding * (-1.0f);

  _vertices = {
      // x axis
      ObjectBase::Vertex(glm::vec3(-_dimensions.x / 2, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
      ObjectBase::Vertex(glm::vec3(_dimensions.x / 2, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),

      // y axis
      ObjectBase::Vertex(glm::vec3(0.0f, -_dimensions.y / 2, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
      ObjectBase::Vertex(glm::vec3(0.0f, _dimensions.y / 2, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),

      // x axis
      ObjectBase::Vertex(glm::vec3(0.0f, 0.0f, -_dimensions.z / 2), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
      ObjectBase::Vertex(glm::vec3(0.0f, 0.0f, _dimensions.z / 2), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
  };

  _indices = {0, 1, 2, 3, 4, 5};
}

CrossHair::~CrossHair() {
}

void CrossHair::render(bool highlighted) {
  // get current line width
  GLfloat fLineWidthSave = 1.0f;
  glGetFloatv(GL_LINE_WIDTH, &fLineWidthSave);

  // set line with to 5 if highlighted is on
  if (highlighted) {
    // std::cout << "highlighted" << std::endl;
    glLineWidth(5.0f);
  } else {
    // std::cout << "not highlighted" << std::endl;
  }

  ObjectBase::render();

  // set back
  if (highlighted) glLineWidth(fLineWidthSave);
}

const glm::vec3& CrossHair::translate(const glm::vec3& translate) {
  glm::vec3 tempPosition = _position + translate;
  bool      inBox        = false;

  if (tempPosition.x > _nhalfBounding.x && tempPosition.x < _halfBounding.x &&
      tempPosition.y > _nhalfBounding.y && tempPosition.y < _halfBounding.y &&
      tempPosition.z > _nhalfBounding.z && tempPosition.z < _halfBounding.z) {
    _position = tempPosition;
  }

  updated_model_matrix();
  return _position;
}
