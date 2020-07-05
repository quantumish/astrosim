#include <vector>
#include <array>
#include <Eigen/Dense>

#define GRAV_CONST (6.674 * pow(10,-11))

class Matter
{
  double mass;
  
  Eigen::Vector3d position;
  Eigen::Vector3d velocity;
  Eigen::Vector3d acceleration;

  Matter(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a);
};

Matter::Matter(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a)
{
  mass = m;
  position = x;
  velocity = v;
  acceleration = a;
}

class Universe
{
  std::vector<Matter> matter;

  void updateMatter(Matter obj);
  void advance();
};
