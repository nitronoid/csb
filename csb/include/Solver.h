#ifndef CSBSOLVER_H
#define CSBSOLVER_H

#include <QOpenGLFunctions>
#include "glm/common.hpp"
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "Particle.h"
#include "PositionConstraint.h"
#include "SimulatedMesh.h"
#include "StaticCollisionConstraint.h"

namespace csb
{

class Solver
{
public:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default constructor.
  //-----------------------------------------------------------------------------------------------------
  Solver() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy constructor.
  //-----------------------------------------------------------------------------------------------------
  Solver(const Solver&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default copy assignment operator.
  //-----------------------------------------------------------------------------------------------------
  Solver& operator=(const Solver&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move constructor.
  //-----------------------------------------------------------------------------------------------------
  Solver(Solver&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default move assignment operator.
  //-----------------------------------------------------------------------------------------------------
  Solver& operator=(Solver&&) = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Default destructor.
  //-----------------------------------------------------------------------------------------------------
  ~Solver() = default;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Adds the given mesh to our simulation.
  /// @param io_mesh is the mesh to add.
  //-----------------------------------------------------------------------------------------------------
  void addTriangleMesh(SimulatedMesh& io_mesh);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Progresses the simulation forwards by the given timestep.
  /// @param _time is the timestep to advance by
  //-----------------------------------------------------------------------------------------------------
  void step(const float _time);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Uses the internal timer to advance the simulation by a fixed timestep.
  //-----------------------------------------------------------------------------------------------------
  void update();

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Iterates over and stores particle indices in the hash table using the hashParticle function.
  /// @param _meshIndex is the index of the referenced mesh, who's particles we should hash.
  //-----------------------------------------------------------------------------------------------------
  void hashVerts(const size_t& _meshIndex);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Iterates over the triangles in a mesh and stores a list of vertices contained within the,
  /// cells that enter the triangles bounding box. The list is stored inside of the triangle-vert hash,
  /// table.
  /// @param _meshIndex is the index of the referenced mesh, who's triangles we should hash.
  //-----------------------------------------------------------------------------------------------------
  void hashTris(const size_t &_meshIndex);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Uses the adjacency list within a mesh to return a list of particle indices that neighbour,
  /// the given particle. These indices are local the the mesh.
  /// @param io_meshRef is the referenced mesh, particles we should query.
  /// @param _particle is the index of the particle, who's neighbours we will retrieve.
  /// @return a list of particle indices, local to io_meshRef, which are neighbours to _vert.
  //-----------------------------------------------------------------------------------------------------
  std::vector<GLushort> getConnectedVertices(SimulatedMesh* io_meshRef, const GLushort _particle);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Attempts to resolve cloth on cloth collisions by calculating line, triangle intersections.
  /// The intersecting points will have their velocities reversed, and their positions reverted to the,
  /// previous ones to simulate a bounce.
  /// @param _meshIndex is the index of the referenced mesh, who's collisions we will resolve.
  //-----------------------------------------------------------------------------------------------------
  void resolveContinuousCollision_rays(const size_t &_meshIndex);
  //-----------------------------------------------------------------------------------------------------
  /// @brief Attempts to resolve cloth on cloth collisions by representing particles as connected spheres,
  /// and checking whether these spheres intersect. Particles that are too close are projected away from,
  /// eachother.
  /// @param _meshIndex is the index of the referenced mesh, who's collisions we will resolve.
  //-----------------------------------------------------------------------------------------------------
  void resolveContinuousCollision_spheres(const size_t &_meshIndex);


  void resolveStaticCollisions(const size_t &_meshIndex);

private:
  //-----------------------------------------------------------------------------------------------------
  /// @brief Stores mesh id and particle index pairs, using a spatial hashing function, this is used to
  /// query neighbours in order to speed up collision detection.
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::vector<std::pair<GLushort, GLushort>>> m_hashTable;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Stores a list of particle id's that neighbour a triangle, the table is indexed using triangle,
  /// id plus an offset for the referenced mesh.
  //-----------------------------------------------------------------------------------------------------
  std::vector<std::vector<size_t>> m_triangleVertHash;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Stores offsets per mesh, that should be used when indexing into the triangle hash table.
  //-----------------------------------------------------------------------------------------------------
  std::vector<size_t> m_triHashOffset;
  //-----------------------------------------------------------------------------------------------------
  /// @brief Stores references to meshes involved in the simulation.
  //-----------------------------------------------------------------------------------------------------
  std::vector<SimulatedMesh*> m_referencedMeshes;

  std::vector<std::unique_ptr<StaticCollisionConstraint>> m_staticCollisions;
  //-----------------------------------------------------------------------------------------------------
  /// @brief An internal timer that is used to ensure the timestep is fixed for the simulation, and that,
  /// the simulation keeps up with the app.
  //-----------------------------------------------------------------------------------------------------
  class FixedTimestepManager
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
  } m_timestepManager;
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

};

}


#endif // CSBSOLVER_H
