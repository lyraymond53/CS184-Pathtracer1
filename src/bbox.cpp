#include "bbox.h"

#include "GL/glew.h"

#include <algorithm>
#include <iostream>

namespace CGL {

bool BBox::intersect(const Ray& r, double& t0, double& t1) const {

  // TODO (Part 2.2):
  // Implement ray - bounding box intersection test
  // If the ray intersected the bounding box within the range given by
  // t0, t1, update t0 and t1 with the new intersection times.
  Vector3D botLeft = this->min;
  Vector3D topRight = this->max;

  double tx1 = (botLeft.x - r.o.x)/r.d.x;
  double ty1 = (botLeft.y - r.o.y)/r.d.y;
  double tz1 = (botLeft.z - r.o.z)/r.d.z;

  double tx2 = (topRight.x -r.o.x)/r.d.x;
  double ty2 = (topRight.y -r.o.y)/r.d.y;
  double tz2 = (topRight.z -r.o.z)/r.d.z;

  double txmin = std::min(tx1, tx2);
  double tymin = std::min(ty1, ty2);
  double tzmin = std::min(tz1, tz2);

  double txmax = std::max(tx1, tx2);
  double tymax = std::max(ty1, ty2);
  double tzmax = std::max(tz1, tz2);

  t0 = std::max(std::max(txmin, tymin), tzmin);
  t1 = std::min(std::min(txmax, tymax), tzmax);

  if (t0 > t1)
  {
    return false;
  }
  return true;
}

void BBox::draw(Color c, float alpha) const {

  glColor4f(c.r, c.g, c.b, alpha);

  // top
  glBegin(GL_LINE_STRIP);
  glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(max.x, max.y, max.z);
  glEnd();

  // bottom
  glBegin(GL_LINE_STRIP);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, min.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, min.y, min.z);
  glEnd();

  // side
  glBegin(GL_LINES);
  glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(min.x, min.y, max.z);
  glEnd();

}

std::ostream& operator<<(std::ostream& os, const BBox& b) {
  return os << "BBOX(" << b.min << ", " << b.max << ")";
}

} // namespace CGL
