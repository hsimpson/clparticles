#pragma once
#include "objectbase.h"

class CrossHair : public ObjectBase {
 private:
  glm::vec3 _dimensions = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 _halfBounding = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 _nhalfBounding = glm::vec3(0.0f, 0.0f, 0.0f);

 public:
  CrossHair(const glm::vec3& dimensions, const glm::vec3& bounding);
  virtual ~CrossHair();
  void                     render(bool highlighted);
  virtual const glm::vec3& translate(const glm::vec3& translate);
};
