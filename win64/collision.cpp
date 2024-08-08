#include "collision.h"


bool ray_intersects_triangle(glm::vec3& ray_origin, glm::vec3& ray_direction, const glm::vec3& p1, const glm::vec3& p2,
                             const glm::vec3& p3, CollisionInfo& out_intersection) {
  const float EPSILON = 0.0000001f;
  glm::vec3 edge1, edge2, h, s, q;
  float a,f,u,v;
  edge1 = p2 - p1;
  edge2 = p3 - p1;
  h = glm::cross(ray_direction, edge2);
  a = glm::dot(edge1, h);
  if (a > -EPSILON && a < EPSILON)
    return false;    // This ray is parallel to this triangle.
  f = 1.0/a;
  s = ray_origin - p1;
  u = f * glm::dot(s, h);
  if (u < 0.0 || u > 1.0)
    return false;
  q = glm::cross(s, edge1);
  v = f * glm::dot(ray_direction, q);
  if (v < 0.0 || u + v > 1.0)
    return false;
  // At this stage we can compute t to find out where the intersection point is on the line.
  float t = f * glm::dot(edge2, q);
  if (t > EPSILON) // ray intersection
  {
    out_intersection.intersection = ray_origin + ray_direction * t;
    out_intersection.distance = t;
    return true;
  }
  else // This means that there is a line intersection but not a ray intersection.
    return false;
}

// Separating Axis Theorem as described in `Real-Time Collision Detection`
bool triangle_intersects_bounding_box(const glm::vec3& v0_in, const glm::vec3& v1_in, const glm::vec3& v2_in, const BoundingBox& bb, CollisionInfo& out_intersection) {
  float p0, p1, p2, r;

  // Compute box center and extents (if not already given in that format)
  glm::vec3 c = (bb.min + bb.max) * 0.5f;
  float e0 = std::abs((bb.max.x - bb.min.x) * 0.5f);
  float e1 = std::abs((bb.max.y - bb.min.y) * 0.5f);
  float e2 = std::abs((bb.max.z - bb.min.z) * 0.5f);  // TODO: This should be positive

  // Translate triangle as conceptually moving AABB to origin
  glm::vec3 v0 = v0_in - c;
  glm::vec3 v1 = v1_in - c;
  glm::vec3 v2 = v2_in - c;

  // Compute edge vectors for triangle
  glm::vec3 f0=v1-v0, f1=v2-v1, f2=v0-v2;

  // Test axes a00..a22 (category 3)
  // Test axis a00
  glm::vec3 a00 = {0.0f, -f0.z, f0.y};
  p0 = glm::dot(v0, a00);
  p2 = glm::dot(v2, a00);
  r = e1 * std::abs(f0.z) + e2 * std::abs(f0.y);
  if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r)
    return false; // Axis is a separating axis

  // Test axis a01
  glm::vec3 a01 = {0.0f, -f1.z, f1.y};
  p0 = glm::dot(v0, a01);
  p1 = glm::dot(v1, a01);  // same
//  p2 = glm::dot(v2, a01);  // same
  r = e1 * std::abs(f1.z) + e2 * std::abs(f1.y);
  if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
    return false; // Axis is a separating axis

  // Test axis a02
  glm::vec3 a02 = {0.0f, -f2.z, f2.y};
  p0 = glm::dot(v0, a02);  // same
  p1 = glm::dot(v1, a02);
//  p2 = glm::dot(v2, a02);  // same
  r = e1 * std::abs(f2.z) + e2 * std::abs(f2.y);
  if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
    return false; // Axis is a separating axis

  // Test axis a10
  glm::vec3 a10 = {f0.z, 0.0f, -f0.x};
  p0 = glm::dot(v0, a10);  // same
//  p1 = glm::dot(v1, a10);  // same
  p2 = glm::dot(v2, a10);
  r = e0 * std::abs(f0.z) + e2 * std::abs(f0.x);
  if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r)
    return false; // Axis is a separating axis

  // Test axis a11
  glm::vec3 a11 = {f1.z, 0.0f, -f1.x};
  p0 = glm::dot(v0, a11);
  p1 = glm::dot(v1, a11);  // same
//  p2 = glm::dot(v2, a11);  // same
  r = e0 * std::abs(f1.z) + e2 * std::abs(f1.x);
  if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
    return false; // Axis is a separating axis

  // Test axis a12
  glm::vec3 a12 = {f2.z, 0.0f, -f2.x};
  p0 = glm::dot(v0, a12);  // same
  p1 = glm::dot(v1, a12);
//  p2 = glm::dot(v2, a12);  // same
  r = e0 * std::abs(f2.z) + e2 * std::abs(f2.x);
  if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
    return false; // Axis is a separating axis

  // Test axis a20
  glm::vec3 a20 = {-f0.y, f0.x, 0.0f};
  p0 = glm::dot(v0, a20);  // same
//  p1 = glm::dot(v1, a20);  // same
  p2 = glm::dot(v2, a20);
  r = e0 * std::abs(f0.y) + e1 * std::abs(f0.x);
  if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r)
    return false; // Axis is a separating axis

  // Test axis a21
  glm::vec3 a21 = {-f1.y, f1.x, 0.0f};
  p0 = glm::dot(v0, a21);
  p1 = glm::dot(v1, a21);  // same
//  p2 = glm::dot(v2, a21);  // same
  r = e0 * std::abs(f1.y) + e1 * std::abs(f1.x);
  if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
    return false; // Axis is a separating axis

  // Test axis a22
  glm::vec3 a22 = {-f2.y, f2.x, 0.0f};
  p0 = glm::dot(v0, a22);  // same
  p1 = glm::dot(v1, a22);
//  p2 = glm::dot(v2, a22);  // same
  r = e0 * std::abs(f2.y) + e1 * std::abs(f2.x);
  if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r)
    return false; // Axis is a separating axis


  // Test the three axes corresponding to the face normals of AABB b (category 1). // Exit if...
  // ... [-e0, e0] and [min(v0.x,v1.x,v2.x), max(v0.x,v1.x,v2.x)] do not overlap if (Max(v0.x, v1.x, v2.x) < -e0 || Min(v0.x, v1.x, v2.x) > e0) return 0;
  // ... [-e1, e1] and [min(v0.y,v1.y,v2.y), max(v0.y,v1.y,v2.y)] do not overlap
  if (std::max(v0.y, std::max(v1.y, v2.y)) < -e1 || std::min(v0.y, std::min(v1.y, v2.y)) > e1)
    return false;
  // ... [-e2, e2] and [min(v0.z,v1.z,v2.z), max(v0.z,v1.z,v2.z)] do not overlap
  if (std::max(v0.z, std::max(v1.z, v2.z)) < -e2 || std::min(v0.z, std::min(v1.z, v2.z)) > e2)
    return false;
  // Test separating axis corresponding to triangle face normal (category 2)

  auto plane_n = glm::cross(f0, f1);
  auto plane_d = std::abs(glm::dot(plane_n, v0));

  // Test AABB-Plane intersection
  // These two lines not necessary with a (center, extents) AABB representation Point c = (b.max + b.min) * 0.5f; // Compute AABB center
  auto e = bb.max - c; // Compute positive extents
  // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
  float radius = e[0]*std::abs(plane_n[0]) + e[1]*std::abs(plane_n[1]) +  (-1.0f)*e[2]*std::abs(plane_n[2]);  // TODO: e[2] is negative here
  // Compute distance of box center from plane
  float s = glm::dot(plane_n, c) - plane_d;
  // Intersection occurs when distance s falls within [-r,+r] interval
  if (std::abs(s) <= radius)
    return true;
  return false;
}

/*
Fast Ray-Box Intersection
by Andrew Woo
from "Graphics Gems", Academic Press, 1990
Implementation from: https://github.com/erich666/GraphicsGems/blob/master/gems/RayBox.c
*/
bool ray_intersects_bounding_box(const glm::vec3& ray_origin, const glm::vec3& ray_direction, const BoundingBox& bb, CollisionInfo& out_intersection) {
  float t1 = (bb.min[0] - ray_origin[0])*(1/ray_direction[0]);
  float t2 = (bb.max[0] - ray_origin[0])*(1/ray_direction[0]);

  float tmin = std::min(t1, t2);
  float tmax = std::max(t1, t2);

  for (int i = 1; i < 3; ++i) {
    t1 = (bb.min[i] - ray_origin[i])*(1/ray_direction[i]);
    t2 = (bb.max[i] - ray_origin[i])*(1/ray_direction[i]);

    tmin = std::max(tmin, std::min(std::min(t1, t2), tmax));
    tmax = std::min(tmax, std::max(std::max(t1, t2), tmin));
  }

  float t = tmax > std::max(tmin, 0.0f) ? tmin : -1;

  if (t >= 0) {
    out_intersection.intersection = ray_origin;
    for (int i = 0; i < 3; ++i) {
      out_intersection.intersection[i] += t * ray_direction[i];
    }

    out_intersection.distance = t;

    return true;
  }

  return false;

//  bool inside = true;
//  char quadrant[3];
//  int i;
//  int whichPlane;
//  double maxT[3];
//  double candidatePlane[3];
//
//  /* Find candidate planes; this loop can be avoided if
//     rays cast all from the eye(assume perpsective view) */
//  for (i=0; i<3; i++)
//    if (ray_origin[i] < bb_min_world[i]) {
//      quadrant[i] = 0;  // left
//      candidatePlane[i] = bb_min_world[i];
//      inside = false;
//    } else if (ray_origin[i] > bb_max_world[i]) {
//      quadrant[i] = 1;  // right
//      candidatePlane[i] = bb_max_world[i];
//      inside = false;
//    } else	{
//      quadrant[i] = 2;  // middle
//    }
//
//  /* Ray origin inside bounding box */
//  if (inside)	{
//    out_intersection.intersection = ray_origin;
//    return true;
//  }
//
//
//  /* Calculate T distances to candidate planes */
//  for (i = 0; i < 3; i++)
//    if (quadrant[i] != 2 && ray_direction[i] != 0.)
//      maxT[i] = (candidatePlane[i]-ray_origin[i]) / ray_direction[i];
//    else
//      maxT[i] = -1.;
//
//  /* Get largest of the maxT's for final choice of intersection */
//  whichPlane = 0;
//  for (i = 1; i < 3; i++)
//    if (maxT[whichPlane] < maxT[i])
//      whichPlane = i;
//
//  /* Check final candidate actually inside box */
//  if (maxT[whichPlane] < 0.) return false;
//  for (i = 0; i < 3; i++)
//    if (whichPlane != i) {
//      out_intersection.intersection[i] = ray_origin[i] + maxT[whichPlane] * ray_direction[i];
//      if (out_intersection.intersection[i] < bb_min_world[i] || out_intersection.intersection[i] > bb_max_world[i])
//        return false;
//    } else {
//      out_intersection.intersection[i] = candidatePlane[i];
//    }
//  return true;				/* ray hits box */
}

//CollisionBox::CollisionBox(int width, int height, int depth) {
//  unsigned int num_vertices = 0;
//
//  build_plane( 2, 1, 0, -1, -1, depth, height, width, &num_vertices ); // px
//  build_plane( 0, 2, 1, 1, 1, width, depth, height, &num_vertices); // py
//  build_plane( 0, 1, 2, 1, -1, width, height, depth, &num_vertices); // pz
//
//  build_plane( 2, 1, 0, 1, -1, depth, height, -width, &num_vertices); // nx
//  build_plane( 0, 2, 1, 1, -1, width, depth, -height, &num_vertices); // ny
//  build_plane( 0, 1, 2, -1, -1, width, height, -depth, &num_vertices); // nz
//}
//
//void CollisionBox::build_plane(int u, int v, int w, int u_dir, int v_dir, int width, int height, int depth, unsigned int* num_vertices) {
//
//  float segment_width = (float)width;
//  float segment_height = (float)height;
//
//  float width_half = (float)width * 0.5f;
//  float height_half = (float)height * 0.5f;
//  float depth_half = (float)depth * 0.5f;
//
//  unsigned int grid_x1 = 1 + 1;
//  unsigned int grid_y1 = 1 + 1;
//
//  int vertex_counter = 0;
//
//  float position[3];
//  float normal[3];
//
//  // generate vertices, normals and uvs
//
//  for ( int iy = 0; iy < grid_y1; iy ++ ) {
//
//    float y = (float)iy * segment_height - height_half;
//
//    for ( int ix = 0; ix < grid_x1; ix ++ ) {
//
//      float x = (float)ix * segment_width - width_half;
//
//      // set values to correct vector component
//
//      position[ u ] = x * (float)u_dir;
//      position[ v ] = y * (float)v_dir;
//      position[ w ] = depth_half;
//
//      // set values to correct vector component
//
//      normal[ u ] = 0;
//      normal[ v ] = 0;
//      normal[ w ] = depth > 0 ? 1 : - 1;
//
//      positions.emplace_back(position[0], position[1], position[2]);
//
//      vertex_counter += 1;
//
//    }
//
//  }
//
//  // indices
//
//  // 1. you need three indices to draw a single face
//  // 2. a single segment consists of two faces
//  // 3. so we need to generate six (2*3) indices per segment
//
//  for ( int iy = 0; iy < 1; iy ++ ) {
//
//    for ( int ix = 0; ix < 1; ix ++ ) {
//
//      unsigned int a = *num_vertices + ix + grid_x1 * iy;
//      unsigned int b = *num_vertices + ix + grid_x1 * ( iy + 1 );
//      unsigned int c = *num_vertices + ( ix + 1 ) + grid_x1 * ( iy + 1 );
//      unsigned int d = *num_vertices + ( ix + 1 ) + grid_x1 * iy;
//
//      // faces
//      indices.insert(indices.end(), {a, b, d});
//      indices.insert(indices.end(), {b, c, d});
//
//    }
//
//  }
//
//  *num_vertices += vertex_counter;
//
//}
//
//void CollisionBox::ray_intersect(glm::vec3& ray_origin, glm::vec3& ray_direction, glm::mat4 world_matrix, std::vector<CollisionInfo>& collisions) {
//  for (unsigned int i = 0; i < indices.size();) {
//    unsigned int ia = indices[i];
//    unsigned int ib = indices[i+1];
//    unsigned int ic = indices[i+2];
//
//    const auto& p1 = glm::vec3(world_matrix * glm::vec4(positions[ia], 1.0f));
//    const auto& p2 = glm::vec3(world_matrix * glm::vec4(positions[ib], 1.0f));
//    const auto& p3 = glm::vec3(world_matrix * glm::vec4(positions[ic], 1.0f));
//
//    CollisionInfo info{};
//
//    if (ray_intersects_triangle(ray_origin, ray_direction, p1, p2, p3, info)) {
//      collisions.push_back(info);
//    }
//
//    i = i+3;
//  }
//}
