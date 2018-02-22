#ifndef CSBMESH_H
#define CSBMESH_H

#include "Mesh.h"

class CSBmesh : public Mesh
{
public:
  void init();
  void update(const float _time);
private:
  std::vector<glm::vec3> m_prevPosition;
};

#endif // CSBMESH_H
