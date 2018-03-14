#pragma once

#include "objectbase.h"

class BoundingBox : public ObjectBase {
 protected:
  glm::vec3 _dimensions;

 public:
  BoundingBox(const glm::vec3& dimensions);
  virtual ~BoundingBox();
};
