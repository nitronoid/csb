#ifndef CSBCONSTRAINT_H
#define CSBCONSTRAINT_H

#include <vector>
#include "vec3.hpp"

class CSBconstraint
{
public:
  CSBconstraint() = default;
  CSBconstraint(const CSBconstraint&) = default;
  CSBconstraint& operator=(const CSBconstraint&) = default;
  CSBconstraint(CSBconstraint&&) = default;
  CSBconstraint& operator=(CSBconstraint&&) = default;
  virtual ~CSBconstraint();
  virtual void project(std::vector<glm::vec3> &_positions) = 0;
};

class DistanceConstraint : public CSBconstraint
{
public:
  DistanceConstraint(const size_t _p1, const size_t _p2, float _distance) :
    m_distance(_distance),
    m_p1(_p1),
    m_p2(_p2)
  {}
  DistanceConstraint(const DistanceConstraint&) = default;
  DistanceConstraint& operator=(const DistanceConstraint&) = default;
  DistanceConstraint(DistanceConstraint&&) = default;
  DistanceConstraint& operator=(DistanceConstraint&&) = default;
  virtual ~DistanceConstraint() override = default;
  virtual void project(std::vector<glm::vec3> &_positions) override;

private:
  float m_distance;
  size_t m_p1;
  size_t m_p2;
};


#endif // CSBCONSTRAINT_H
