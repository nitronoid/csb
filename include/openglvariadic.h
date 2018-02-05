#ifndef OPENGLVARIADIC_H
#define OPENGLVARIADIC_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <type_traits>
#include <utility>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

namespace glv
{
template<size_t len, size_t required>
using MatchSize = typename std::enable_if_t<len == required>;

template<typename A, typename B>
using MatchType = typename std::enable_if_t<std::is_same<A, B>::value>;

template<typename A, typename B>
using MatchTypeNot = typename std::enable_if_t<!std::is_same<A, B>::value>;


//-----------------------------------------------------------------------------------------------------------------
//------------------------------BEGIN GLFLOAT DISPATCH-------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
template<size_t k_numArgs, typename ...Args, MatchSize<k_numArgs, 1>* = nullptr>
void glUniformNumDispatch(const GLint _name, const GLfloat arg, Args&& ...rest)
{
  glUniform1f(_name, arg, std::forward<Args>(rest)...);
}

template<size_t k_numArgs, typename ...Args, MatchSize<k_numArgs, 2>* = nullptr>
void glUniformNumDispatch(const GLint _name, const GLfloat arg, Args&& ...rest)
{
  glUniform2f(_name, arg, std::forward<Args>(rest)...);
}

template<size_t k_numArgs, typename ...Args, MatchSize<k_numArgs, 3>* = nullptr>
void glUniformNumDispatch(const GLint _name, const GLfloat arg, Args&& ...rest)
{
  glUniform3f(_name, arg, std::forward<Args>(rest)...);
}

template<size_t k_numArgs, typename ...Args, MatchSize<k_numArgs, 4>* = nullptr>
void glUniformNumDispatch(const GLint _name, const GLfloat arg, Args&& ...rest)
{
  glUniform4f(_name, arg, std::forward<Args>(rest)...);
}

template<typename ...Args>
void glUniformTypeDispatch(const GLint _name, const GLfloat arg, Args&& ...rest)
{
  constexpr size_t num = {sizeof...(rest) + 1};
  glUniformNumDispatch<num>(_name, arg, std::forward<Args>(rest)...);
}
//-----------------------------------------------------------------------------------------------------------------
//-------------------------------END GLFLOAT DISPATCH--------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------
//------------------------------BEGIN MATRIX DISPATCH--------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

template<typename T>
void glUniformMatDispatch(const GLint _name, const glm::tmat4x4<T>& arg, MatchType<T, float>* = nullptr)
{
  glUniformMatrix4fv(_name, 1, GL_FALSE, glm::value_ptr(arg));
}

template<typename T>
void glUniformMatDispatch(const GLint _name, const glm::tmat4x4<T>& arg, MatchType<T, double>* = nullptr)
{
  glUniformMatrix4dv(_name, 1, GL_FALSE, glm::value_ptr(arg));
}

template<typename T>
void glUniformTypeDispatch(const GLint _name, const glm::tmat4x4<T>& arg)
{
  glUniformMatDispatch(_name, arg);
}
//-----------------------------------------------------------------------------------------------------------------
//--------------------------------END MATRIX DISPATCH--------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------


template<typename ...Args>
void glUniform(const GLint _name, Args&& ...args)
{
  glUniformTypeDispatch(_name, std::forward<Args>(args)...);
}

}

#endif // OPENGLVARIADIC_H
