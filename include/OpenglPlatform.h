#ifndef OPENGLPLATFORM_H
#define OPENGLPLATFORM_H

// OpenGL includes
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#define LINUX
#endif

#endif // OPENGLPLATFORM_H
