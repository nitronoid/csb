#ifndef CSBMESH_H
#define CSBMESH_H

#include "Mesh.h"
#include "CSBconstraint.h"

class CSBmesh : public Mesh
{
public:
  void init();
  void update(const float _time);
private:
  std::vector<glm::vec3> m_prevPosition;
  std::vector<std::unique_ptr<CSBconstraint>> m_constraints;
};

#endif // CSBMESH_H
