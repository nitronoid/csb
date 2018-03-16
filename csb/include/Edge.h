#ifndef EDGE_H
#define EDGE_H

#include <QOpenGLFunctions>

namespace csb
{

struct Edge
{
  Edge(const GLushort _a, const GLushort _b) :
    p(std::min(_a, _b), std::max(_a, _b))
  {}
  friend bool operator==(const Edge &_a, const Edge &_b)
  {
    return _a.p == _b.p;
  }
  std::pair<GLushort, GLushort> p;
};

}

namespace std
{
template <>
struct hash<csb::Edge>
{
  size_t operator()(const csb::Edge &_key) const
  {
    return std::hash<size_t>()(std::hash<GLushort>()(_key.p.first)) ^ std::hash<GLushort>()(_key.p.second);
  }
};
}

#endif // EDGE_H
