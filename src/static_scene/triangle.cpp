#include "triangle.h"

#include "CGL/CGL.h"
#include "GL/glew.h"

namespace CGL { namespace StaticScene {

Triangle::Triangle(const Mesh* mesh, size_t v1, size_t v2, size_t v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3) { }

BBox Triangle::get_bbox() const {

  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  BBox bb(p1);
  bb.expand(p2); 
  bb.expand(p3);
  return bb;

}

bool Triangle::intersect(const Ray& r) const {

  // TODO (Part 1.3):
  // implement ray-triangle intersection

  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);

  Vector3D origin = r.o;
  Vector3D direction = r.d;

  Vector3D E1 = p2 - p1;
  Vector3D E2 = p3 - p1;
  Vector3D S = origin - p1;
  Vector3D S1 = cross(direction, E2);
  Vector3D S2 = cross(S, E1);

  Vector3D output = (1/dot(S1, E1)) * Vector3D(dot(S2, E2), dot(S1, S), dot(S2, direction));

  if ((r.min_t <= output.x) && (output.x <= r.max_t))
  {
    r.max_t = output.x;
    return true;
  }
  return false;
}

bool Triangle::intersect(const Ray& r, Intersection *isect) const {
  
  // TODO (Part 1.3):
  // implement ray-triangle intersection. When an intersection takes
  // place, the Intersection data should be updated accordingly

  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  Vector3D n1(mesh->normals[v1]), n2(mesh->normals[v2]), n3(mesh->normals[v3]);

  Vector3D origin = r.o;
  Vector3D direction = r.d;

  Vector3D E1 = p2 - p1;
  Vector3D E2 = p3 - p1;
  Vector3D S = origin - p1;
  Vector3D S1 = cross(direction, E2);
  Vector3D S2 = cross(S, E1);

  Vector3D output = (1/dot(S1, E1)) * Vector3D(dot(S2, E2), dot(S1, S), dot(S2, direction));

  Vector3D bary = Vector3D(output.y, output.z, 1-output.y-output.z);

  if (bary.x < 0 || bary.x > 1 || bary.y < 0 || bary.y > 1 || bary.z < 0 || bary.z > 1)
  {
      return false;
  }

  if ((r.min_t <= output.x) && (output.x <= r.max_t))
  {
    r.max_t = output.x;
    isect->t = output.x;

    Vector3D normal = bary.z*n1 + bary.x*n2 + bary.y*n3;

    isect->n = normal;

    isect->primitive = this;
    isect->bsdf = get_bsdf();
    return true;
  }
  return false;
}

void Triangle::draw(const Color& c, float alpha) const {
  glColor4f(c.r, c.g, c.b, alpha);
  glBegin(GL_TRIANGLES);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

void Triangle::drawOutline(const Color& c, float alpha) const {
  glColor4f(c.r, c.g, c.b, alpha);
  glBegin(GL_LINE_LOOP);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}



} // namespace StaticScene
} // namespace CGL
