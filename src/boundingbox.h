#pragma once

#include "objectbase.h"

class BoundingBox : public ObjectBase {
 protected:
  glm::vec3 _dimensions = glm::vec3(0.0f, 0.0f, 0.0f);

 public:
  BoundingBox(const glm::vec3& dimensions);
  virtual ~BoundingBox();
};
