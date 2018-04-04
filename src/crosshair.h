#pragma once
#include "objectbase.h"

class CrossHair : public ObjectBase {
 private:
  glm::vec3 _dimensions;
  glm::vec3 _halfBounding;
  glm::vec3 _nhalfBounding;

 public:
  CrossHair(const glm::vec3& dimensions, const glm::vec3& bounding);
  virtual ~CrossHair();
  void                     render(bool highlighted);
  virtual const glm::vec3& translate(const glm::vec3& translate);
};
