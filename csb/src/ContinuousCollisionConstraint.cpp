#include "ContinuousCollisionConstraint.h"


//----------------------------------------------------------------------------------------------------------------------------
csb::ContinuousCollisionConstraint::~ContinuousCollisionConstraint() = default;
//----------------------------------------------------------------------------------------------------------------------------
std::vector<csb::Particle>& csb::ContinuousCollisionConstraint::getParticles(SimulatedMesh&_mesh)  const noexcept
{
  return _mesh.m_particles;
}
//----------------------------------------------------------------------------------------------------------------------------
