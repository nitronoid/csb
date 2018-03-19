# Cloth & Soft Bodies

A cloth and soft bodies simulation library, using Position Based Dynamics.
Uses triangle mesh .obj files, and offers a constraint based system for simulating cloth like behaviour.

# Installation
## Dependencies

- C++14
- OpenGL 4.1
- GLM (9.8.5 was used)
- Assimp 
- Qt 5.9 (used for OpenGL functions)
- gtest

## Building and running

The top level directory contains three sub directories, csb, demo and tests. 
In the csb folder you will find the library code. Each project can be build individually, however tests and demo are dependent on the library.
The top level directory also contains a buildall.pro which will build all three,
sub directories in the correct order.

The simplest way to view demos is to:
1. Go to the top level directory.
2. qmake
3. make -j
4. Change into the demos/ directory
5. ./Demo

## Tests

The tests have been written with gtest and can be found in the tests/ sub directory.
If you performed the steps above, you can run ./CSBTests, otherwise you will need to first, build the library then build the tests.

# User Instructions

## Demos
Within the demo's project you can see multiple demo's set up in functions of the DemoScene class. You are required to manually change the currently set demo in the code
by changing the call to initDemoX(), in the init function, where X is the demo you want to run.

### Controls

The demo makes use of Qt's gui system and the widgets should be clear.
The demo does have mouse controls:

- Right click and drag - Zoom camera
- Left click and drag - Orbit camera

# Creating a simulation

To set up a simulation using the library we first require a Solver.

```c++
#include "Solver.h"

//...

csb::Solver solver;
```

Next you will want to load up the meshes that you want to simulate as cloth. The solver uses shared pointers to meshes to ensure that the lifetimes do not fall short.
The init call is separate from the load function as you are able to load meshes from multiple files into the same mesh object, so multiple load calls are permitted, however the mesh should be reset before call init more than once.

```c++
#include "SimulatedMesh.h"

//...

std::shared_ptr<csb::SimulatedMesh> mesh = std::make_shared<csb::SimulatedMesh>();
mesh->load("path/to/mesh.obj");
mesh->init();
```

You can then add constraints to the mesh, the library includes some presets such as ```generateStructuralConstraints()```, ```generateBendingConstraints(float)``` and ```generateClothConstraints(float)``` which is a wrapper for the first two. These can be used to automatically generate constraints across all edges of the mesh and neighbouring vertices to easily create a cloth mesh. You can also set the particle masses. It is important to set particle masses before creating constraints.


```c++
//The mesh will hang from it's first vertex
mesh->setParticleInverseMass(0, 0.f);
// Generate cloth constraints, where the bending stiffness is 0.05f
mesh->generateClothConstraints(0.05f);
```

Then you should add the mesh to the simulation by giving it to the solver.
```c++
solver.addSimulatedMesh(mesh);
```

The solver also has constraints which can be set, these constraints will be global to the simulation and affect all meshes.

```c++
// Adds a self collision constraint that uses ray triangle intersection checks
solver.addContinuousCollision(new csb::SelfCollisionRaysConstraint);
// Adds a static sphere obstacle to the simulation
solver.addStaticCollision(new csb::SphereCollisionConstraint({0.f,-1.f,0.f}, 0.5f));
```

You can also set some simulation parameters.

```c++
// Adds a gravity to the simulation
solver.addForce({0.f, -9.81f, 0.f});
// Drag can be simulated with slight damping      
solver.setDamping(0.1f);                   
// The mesh constraints will be solved 60 times per step
solver.setPositionConstraintIterations(60);
```

Finally you are ready to simulate. The solver has an in-built timer that manages a fixed time-step. This timer is used internally when you call the ```update()``` function. If you prefer to write your own timer you can call the ```step(float dt)``` function directly.

```c++
solver.update();
// or with your own timer
solver.step(TIME_STEP);
```

The last thing you will want to do is draw your simulated meshes. The meshes are stored using indexed vertices, and so you are required to use glDrawElements rather than arrays. The meshes also store indices independently of each-other and so you must use the BaseVertex gl draw calls. If you have an array of meshes an example would be:

```c++
glMultiDrawElementsBaseVertex(
        GL_TRIANGLES,
        // This would store mesh[i-1]->getNIndices() for every ith mesh
        m_numIndicesPerMesh.data(),
        GL_UNSIGNED_SHORT,
        // This would store the addresses into your vbo that every mesh's indices begin
        m_meshIndexOffsets.data(),
        // Number of meshes to draw
        static_cast<GLsizei>(meshes.size()),
        // This would store mesh[i-1]->getNVerts() for every ith mesh
        m_meshBaseVert.data()
        );
```

# Limitations

The systems self collision detection is not perfect and can create tangled cloth in some cases, so if self collision will not be extreme, you may get better results when not using a SelfCollisionRaysConstraint.