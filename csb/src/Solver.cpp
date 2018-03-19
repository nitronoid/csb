#include "Solver.h"
#include "SpatialHash.h"
#include "PositionConstraint.h"


//----------------------------------------------------------------------------------------------------------------------------
struct csb::Solver::SolverImpl
{
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  SolverImpl() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Copy constructor.
  //-----------------------------------------------------------------------------------------------------
  SolverImpl(const SolverImpl&_rhs);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SolverImpl& operator=(const SolverImpl&_rhs);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  SolverImpl(SolverImpl&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  SolverImpl& operator=(SolverImpl&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default destructor.
  //-----------------------------------------------------------------------------------------------------
  ~SolverImpl() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The spatial hashing information for the sim.
  //-----------------------------------------------------------------------------------------------------
  SpatialHash::SpatialHashTable m_spatialHash;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Stores shared references to meshes involved in the simulation, this means the meshes will,
  /// be kept alive for the solvers entire lifetime.
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::shared_ptr<SimulatedMesh>> m_referencedMeshes;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Stores the continuous collision constraints.
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::unique_ptr<ContinuousCollisionConstraint>> m_continuousCollisions;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Stores the static collision constraints.
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::unique_ptr<StaticCollisionConstraint>> m_staticCollisions;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The total number of particles in the simulation.
  //-----------------------------------------------------------------------------------------------------
  size_t m_numParticles = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The total number of triangles in the simulation.
  //-----------------------------------------------------------------------------------------------------
  size_t m_numTris = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The total number of mesh edges in the simulation.
  //-----------------------------------------------------------------------------------------------------
  size_t m_numEdges = 0;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The force that should be used to calculate accel during the intergration step.
  //-----------------------------------------------------------------------------------------------------
  glm::vec3 m_totalForce {0.f};
  //-----------------------------------------------------------------------------------------------------
  /// @brief The amount of times the position constraints should be projected per timestep. This controls,
  /// how stretchy the cloth appears.
  //-----------------------------------------------------------------------------------------------------
  unsigned int m_positionConstraintIterations = 1;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The shortest edge length, used for sphere collision.
  //-----------------------------------------------------------------------------------------------------
  float m_shortestEdgeLength = std::numeric_limits<float>::max();
  //-----------------------------------------------------------------------------------------------------
  /// @brief The combined sum of all edge lengths, used for calculating the average edge length.
  //-----------------------------------------------------------------------------------------------------
  float m_totalEdgeLength = 0.0f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The average edge length, used for optimal spatial hashing cell size.
  //-----------------------------------------------------------------------------------------------------
  float m_averageEdgeLength = 0.0f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The amount of velocity damping to apply during the integration step.
  //-----------------------------------------------------------------------------------------------------
  float m_dampingComplement = 1.f;
};
//----------------------------------------------------------------------------------------------------------------------------
csb::Solver::SolverImpl::SolverImpl(const SolverImpl&_rhs) :
  m_spatialHash(_rhs.m_spatialHash),
  m_numParticles(_rhs.m_numParticles),
  m_numTris(_rhs.m_numTris),
  m_numEdges(_rhs.m_numEdges),
  m_totalForce(_rhs.m_totalForce),
  m_shortestEdgeLength(_rhs.m_shortestEdgeLength),
  m_totalEdgeLength(_rhs.m_totalEdgeLength),
  m_averageEdgeLength(_rhs.m_averageEdgeLength),
  m_dampingComplement(_rhs.m_dampingComplement)
{
  // Deep copy the meshes
  for (auto& mesh : _rhs.m_referencedMeshes)
    m_referencedMeshes.emplace_back(std::make_shared<csb::SimulatedMesh>(*mesh));

  for (auto& staticCollision : _rhs.m_staticCollisions)
    m_staticCollisions.emplace_back(staticCollision->clone());
}
//----------------------------------------------------------------------------------------------------------------------------
csb::Solver::SolverImpl& csb::Solver::SolverImpl::operator=(const SolverImpl&_rhs)
{
  m_spatialHash =_rhs.m_spatialHash;
  m_numParticles =_rhs.m_numParticles;
  m_numTris = _rhs.m_numTris;
  m_numEdges = _rhs.m_numEdges;
  m_totalForce = _rhs.m_totalForce;
  m_shortestEdgeLength = _rhs.m_shortestEdgeLength;
  m_totalEdgeLength = _rhs.m_totalEdgeLength;
  m_averageEdgeLength = _rhs.m_averageEdgeLength;
  m_dampingComplement = _rhs.m_dampingComplement;

  // Deep copy the meshes
  for (auto& mesh : _rhs.m_referencedMeshes)
    m_referencedMeshes.emplace_back(std::make_shared<csb::SimulatedMesh>(*mesh));

  for (auto& staticCollision : _rhs.m_staticCollisions)
    m_staticCollisions.emplace_back(staticCollision->clone());

  return *this;
}
//-----------------------------------------------------------------------------------------------------
/// @brief An internal timer that is used to ensure the timestep is fixed for the simulation, and that,
/// the simulation keeps up with the app.
//-----------------------------------------------------------------------------------------------------
class csb::Solver::FixedTimestepManager
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Progresses the timer and accumulates the time we have to simulate for. This should be called,
  /// once prior to calling step.
  //-----------------------------------------------------------------------------------------------------
  void progress();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Consumes a time step from our accumulated time. This should be called until the returned,
  /// result is false, each time the simulation should be stepped forward.
  /// @return Whether we are more than a timestep behind.
  //-----------------------------------------------------------------------------------------------------
  bool consume();
  //-----------------------------------------------------------------------------------------------------
  /// @brief Read only access to the timestep.
  /// @return The timestep that should be used to step the simulation forward.
  //-----------------------------------------------------------------------------------------------------
  const float& deltaTime();

private:
  //-----------------------------------------------------------------------------------------------------
  /// @note Using declaration for readability.
  //-----------------------------------------------------------------------------------------------------
  using hr_clock = std::chrono::high_resolution_clock;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The time that progress was last called.
  //-----------------------------------------------------------------------------------------------------
  hr_clock::time_point m_lastTime;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The fixed timestep.
  //-----------------------------------------------------------------------------------------------------
  float m_timestep = 1.0f / 30.f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief The accumulated simulation time to be consumed.
  //-----------------------------------------------------------------------------------------------------
  float m_accum = 0.0f;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Used for lazy initialisation of m_lastTime.
  //-----------------------------------------------------------------------------------------------------
  bool m_isUsed = false;
};
//----------------------------------------------------------------------------------------------------------------------------
csb::Solver::Solver() :
  m_impl(std::make_unique<SolverImpl>()),
  m_timestepManager(std::make_unique<FixedTimestepManager>())
{}
//----------------------------------------------------------------------------------------------------------------------------
csb::Solver::Solver(const Solver&_rhs) :
  m_impl(new SolverImpl(*_rhs.m_impl)),
  m_timestepManager(new FixedTimestepManager(*_rhs.m_timestepManager))
{}
//----------------------------------------------------------------------------------------------------------------------------
csb::Solver& csb::Solver::operator=(const Solver&_rhs)
{
  m_impl.reset(new SolverImpl(*_rhs.m_impl));
  m_timestepManager.reset(new FixedTimestepManager(*_rhs.m_timestepManager));
  return *this;
}
//----------------------------------------------------------------------------------------------------------------------------
csb::Solver::Solver(Solver&&) = default;
//----------------------------------------------------------------------------------------------------------------------------
csb::Solver& csb::Solver::operator=(Solver&&) = default;
//----------------------------------------------------------------------------------------------------------------------------
csb::Solver::~Solver() = default;
//----------------------------------------------------------------------------------------------------------------------------
std::vector<GLushort> csb::Solver::getConnectedVertices(const std::shared_ptr<SimulatedMesh> &io_meshRef, const GLushort _particle)
{
  return io_meshRef->getAdjacencyInfo()[_particle];
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::hashVerts(const size_t &_meshIndex)
{
  const auto mesh = m_impl->m_referencedMeshes[_meshIndex];
  const auto size = mesh->getNVerts();
  auto& hashTable = m_impl->m_spatialHash.m_hashTable;
  const auto tableSize = hashTable.size();
  for (GLushort i = 0; i < size; ++i)
  {
    hashTable[SpatialHash::hashParticle(*mesh->m_particles[i].m_pos, tableSize, m_impl->m_averageEdgeLength, m_impl->m_spatialHash.m_cellOffset)].emplace_back(_meshIndex, i);
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::hashTris(const size_t &_meshIndex)
{
  const auto mesh = m_impl->m_referencedMeshes[_meshIndex];
  const auto hashOffset = m_impl->m_spatialHash.m_triHashOffset[_meshIndex];
  const auto size = mesh->getNIndices() / 3;
  const auto& indices = mesh->getIndices();
  for (size_t i = 0; i < size; ++i)
  {
    const size_t index = i * 3;
    const auto& p1 = mesh->m_particles[indices[index]];
    const auto& p2 = mesh->m_particles[indices[index + 1]];
    const auto& p3 = mesh->m_particles[indices[index + 2]];

    const auto min = SpatialHash::calcCell(glm::min(glm::min(*p1.m_pos, *p2.m_pos), *p3.m_pos), m_impl->m_averageEdgeLength, m_impl->m_spatialHash.m_cellOffset);
    const auto max = SpatialHash::calcCell(glm::max(glm::max(*p1.m_pos, *p2.m_pos), *p3.m_pos), m_impl->m_averageEdgeLength, m_impl->m_spatialHash.m_cellOffset);

    // hash all cells within the bounding box of this triangle
    for (int x = min.x; x <= max.x; ++x)
      for (int y = min.y; y <= max.y; ++y)
        for (int z = min.z; z <= max.z; ++z)
        {
          m_impl->m_spatialHash.m_triangleBoundingBoxHash[i + hashOffset].push_back(SpatialHash::hashCell({x,y,z}, m_impl->m_spatialHash.m_hashTable.size()));
        }
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::resolveStaticCollisions(const size_t &_meshIndex)
{
  for (auto& collisionConstraint : m_impl->m_staticCollisions)
  {
    collisionConstraint->project(m_impl->m_referencedMeshes[_meshIndex]->m_particles, m_impl->m_spatialHash);
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::resolveContinuousCollisions(const size_t &_meshIndex)
{
  for (auto& collisionConstraint : m_impl->m_continuousCollisions)
  {
    collisionConstraint->project(_meshIndex, m_impl->m_referencedMeshes, m_impl->m_spatialHash);
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::addStaticCollision(StaticCollisionConstraint* _newConstraint)
{
  _newConstraint->setCellSize(m_impl->m_averageEdgeLength);
  _newConstraint->setHashTableSize(m_impl->m_spatialHash.m_hashTable.size());
  _newConstraint->setCellOffset(m_impl->m_spatialHash.m_cellOffset);
  _newConstraint->init();
  m_impl->m_staticCollisions.emplace_back(_newConstraint);
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::addContinuousCollision(ContinuousCollisionConstraint* _newConstraint)
{
  m_impl->m_continuousCollisions.emplace_back(_newConstraint);
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::addSimulatedMesh(const std::shared_ptr<SimulatedMesh> &io_mesh)
{
  // Store a reference pointer to the mesh
  m_impl->m_referencedMeshes.push_back(io_mesh);
  m_impl->m_spatialHash.m_triHashOffset.push_back(m_impl->m_numTris);
  // Update our stored counters
  m_impl->m_numParticles += io_mesh->getNVerts();
  m_impl->m_numTris += io_mesh->getNIndices() / 3;
  m_impl->m_totalEdgeLength += io_mesh->getTotalEdgeLength();
  m_impl->m_numEdges += io_mesh->getNEdges();
  m_impl->m_averageEdgeLength = (m_impl->m_totalEdgeLength + io_mesh->getTotalEdgeLength()) / m_impl->m_numEdges;
  m_impl->m_shortestEdgeLength = std::min(m_impl->m_shortestEdgeLength, io_mesh->getShortestEdgeLength());

  // Calculate optimal hash table size
  m_impl->m_spatialHash.m_triangleBoundingBoxHash.resize(m_impl->m_numTris);
  const size_t multiple = static_cast<size_t>(pow10(floor(log10(m_impl->m_numParticles))));
  const auto hashTableSize = ((m_impl->m_numParticles + multiple - 1) / multiple) * multiple - 1;
  m_impl->m_spatialHash.m_hashTable.resize(hashTableSize);
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::FixedTimestepManager::progress()
{
  const auto currentTime = hr_clock::now();

  // Lazy initialise current time to avoid a spike when the sim begins
  if (!m_isUsed)
  {
    m_lastTime = currentTime;
    m_isUsed = true;
  }

  using namespace std::chrono;
  float ft = duration_cast<milliseconds>(currentTime - m_lastTime).count() / 1000.0f;
  m_lastTime = currentTime;
  m_accum += ft;
}
//----------------------------------------------------------------------------------------------------------------------------
bool csb::Solver::FixedTimestepManager::consume()
{
  const auto isBehind = m_accum >= m_timestep;
  if (isBehind) m_accum -= m_timestep;
  return isBehind;
}
//----------------------------------------------------------------------------------------------------------------------------
const float& csb::Solver::FixedTimestepManager::deltaTime()
{
  return m_timestep;
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::update()
{
  m_timestepManager->progress();
  while (m_timestepManager->consume())
  {
    step(m_timestepManager->deltaTime());
  }
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::step(const float _time)
{

  for (unsigned int i = 0; i < m_impl->m_positionConstraintIterations; ++i)
    for (auto mesh : m_impl->m_referencedMeshes)
      mesh->projectConstraints();


  for (auto& hash : m_impl->m_spatialHash.m_triangleBoundingBoxHash) hash.clear();
  for (auto& cell : m_impl->m_spatialHash.m_hashTable) cell.clear();

  const auto numMeshes = m_impl->m_referencedMeshes.size();
  for (size_t i = 0; i < numMeshes; ++i)
  {
    hashVerts(i);
    hashTris(i);
  }

  for (size_t i = 0; i < numMeshes; ++i)
  {
    resolveStaticCollisions(i);
    resolveContinuousCollisions(i);
  }

  for (auto mesh : m_impl->m_referencedMeshes)
  {
    for (auto& particle : mesh->m_particles)
    {
      const auto accel = particle.m_invMass * m_impl->m_totalForce;
      const auto vel = (*particle.m_pos - particle.m_prevPos) * m_impl->m_dampingComplement;
      const auto newPos = *particle.m_pos + vel + (accel * _time * _time);
      particle.m_prevPos = *particle.m_pos;
      *particle.m_pos = newPos;
    }
  }

}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::addForce(const glm::vec3 &_force)
{
  m_impl->m_totalForce += _force;
}
//----------------------------------------------------------------------------------------------------------------------------
glm::vec3 csb::Solver::getTotalForce() const noexcept
{
  return m_impl->m_totalForce;
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::setTotalForce(const glm::vec3 &_force)
{
  m_impl->m_totalForce = _force;
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::setDamping(const float _damping)
{
  m_impl->m_dampingComplement = glm::clamp(1.f - _damping, 0.0f, 1.f);
}
//----------------------------------------------------------------------------------------------------------------------------
float csb::Solver::getDamping() const noexcept
{
  return 1.f - m_impl->m_dampingComplement;
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::setPositionConstraintIterations(const unsigned int _iterations)
{
  m_impl->m_positionConstraintIterations = _iterations;
}
//----------------------------------------------------------------------------------------------------------------------------
unsigned int csb::Solver::getPositionConstraintIterations() const noexcept
{
  return m_impl->m_positionConstraintIterations;
}
//----------------------------------------------------------------------------------------------------------------------------
void csb::Solver::setCellOffset(const float _offset)
{
  m_impl->m_spatialHash.m_cellOffset = _offset;
}
//----------------------------------------------------------------------------------------------------------------------------
float csb::Solver::getCellOffset() const noexcept
{
  return m_impl->m_spatialHash.m_cellOffset;
}
//----------------------------------------------------------------------------------------------------------------------------
