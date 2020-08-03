#include <vector>
#include <array>
#include <iostream>
#include <cmath>

#define MOLE (6.02214076*pow(10,23))

class Matter;

struct GenericForce
{
};

template <class T1, class T2>
struct Force : GenericForce
{
public:
  Eigen::Vector3d components;
  T1* source;
  T2* target;
  Force();
  Force(T1* src, T2* end, Eigen::Vector3d parts);
};

template<class T1, class T2>
Force<T1,T2>::Force()
  :components{0,0,0}, target{nullptr}, source{nullptr}
{
}

template<class T1, class T2>
Force<T1,T2>::Force(T1* src, T2* end, Eigen::Vector3d parts)
  :components{parts}, target{end}, source{src}
{
}

class Matter : public Body
{
public:
  double radius;
  Matter(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a);
};

Matter::Matter(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a)
  :Body(m,x,v,a), radius{r}
{
  assert (radius > 0);
}

class Photon
{
public:
  Eigen::Vector3d position;
  Eigen::Vector3d direction; // Unit vector in correct direction
  Photon(Eigen::Vector3d x, Eigen::Vector3d d);
};

Photon::Photon(Eigen::Vector3d x, Eigen::Vector3d d)
  :position{x}, direction{d}
{
}
