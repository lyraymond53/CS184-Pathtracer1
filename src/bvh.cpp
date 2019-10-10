#include "bvh.h"

#include "CGL/CGL.h"
#include "static_scene/triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CGL { namespace StaticScene {

BVHAccel::BVHAccel(const std::vector<Primitive *> &_primitives,
                   size_t max_leaf_size) {

  root = construct_bvh(_primitives, max_leaf_size);

}

BVHAccel::~BVHAccel() {
  if (root) delete root;
}

BBox BVHAccel::get_bbox() const {
  return root->bb;
}

void BVHAccel::draw(BVHNode *node, const Color& c, float alpha) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->draw(c, alpha);
  } else {
    draw(node->l, c, alpha);
    draw(node->r, c, alpha);
  }
}

void BVHAccel::drawOutline(BVHNode *node, const Color& c, float alpha) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->drawOutline(c, alpha);
  } else {
    drawOutline(node->l, c, alpha);
    drawOutline(node->r, c, alpha);
  }
}

BVHNode *BVHAccel::construct_bvh(const std::vector<Primitive*>& prims, size_t max_leaf_size) {
  
  // TODO (Part 2.1):
  // Construct a BVH from the given vector of primitives and maximum leaf
  // size configuration. The starter code build a BVH aggregate with a
  // single leaf node (which is also the root) that encloses all the
  // primitives.

  BBox centroid_box, bbox;

  vector<Primitive *> left;
  vector<Primitive *> right;

  Vector3D split = Vector3D(0, 0, 0);

  for (Primitive *p : prims) {
      BBox bb = p->get_bbox();
      bbox.expand(bb);
      Vector3D c = bb.centroid();
      centroid_box.expand(c);
      split = split + p->get_bbox().centroid();
  }

  split = split/(double) prims.size();

  BVHNode *node = new BVHNode(bbox);

  if (prims.size() <= max_leaf_size) {
    node->prims = new vector<Primitive *>(prims);
    return node;
  } else {
    //Divide the sets into left and right based on largest dimension of bounding box extent

    //Get the largest dimension of bbox extent
    double maxDim = bbox.extent[0];
    int maxIndex = 0;
    for (int i = 1; i < 3; i++)
    {
        if (bbox.extent[i] > maxDim)
        {
            maxIndex = i;
            maxDim = bbox.extent[i];
        }
    }

    //Sort each prim into left or right primitive vectors
    for (Primitive* p: prims)
    {
      if (p->get_bbox().centroid()[maxIndex] < split[maxIndex]) {
        left.push_back(p);
      }
      else {
        right.push_back(p);
      }
    }
  }
  node->l = construct_bvh(left, max_leaf_size);
  node->r = construct_bvh(right, max_leaf_size);

  node->prims = new vector<Primitive *>(prims);
  return node;
}


bool BVHAccel::intersect(const Ray& ray, BVHNode *node) const {

  // TODO (Part 2.3):
  // Fill in the intersect function.
  // Take note that this function has a short-circuit that the
  // Intersection version cannot, since it returns as soon as it finds
  // a hit, it doesn't actually have to find the closest hit.

//  for (Primitive *p : *(root->prims)) {
//    total_isects++;
//    if (p->intersect(ray))
//      return true;
//  }
//  return false;
//
//
    double t0 = ray.min_t;
    double t1 = ray.max_t;

    if (!node->bb.intersect(ray, t0, t1))
    {
        return false;
    }
    if (node->isLeaf())
    {
        bool hit = false;
        for (Primitive* p: *(node->prims))
        {
            total_isects++;
            if (p->intersect(ray))
            {
                hit = true;
            }
        }
        return hit;
    }
    else
    {
        bool hit1 = intersect(ray, node->l);
        bool hit2 = intersect(ray, node->r);
        return hit1 || hit2;
    }
}

bool BVHAccel::intersect(const Ray& ray, Intersection* i, BVHNode *node) const {

  // TODO (Part 2.3):
  // Fill in the intersect function.

//  bool hit = false;
//  for (Primitive *p : *(root->prims)) {
//    total_isects++;
//    if (p->intersect(ray, i))
//      hit = true;
//  }
//  return hit;

    double t0 = ray.min_t;
    double t1 = ray.max_t;

    if (!node->bb.intersect(ray, t0, t1))
    {
        return false;
    }
    if (node->isLeaf())
    {
        bool hit = false;
        for (Primitive* p: *(node->prims))
        {
            total_isects++;
            if (p->intersect(ray, i))
            {
                hit = true;
            }
        }
        return hit;
    }
    else
    {
        bool hit1 = intersect(ray, i, node->l);
        bool hit2 = intersect(ray, i, node->r);
        return hit1 || hit2;
    }
}

}  // namespace StaticScene
}  // namespace CGL
