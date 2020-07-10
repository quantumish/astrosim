#include <vector>
#include <array>
#include <Eigen/Dense>

class Photometer
{
public:
  double radius;
  Eigen::Vector3d position;
  std::vector<double> recorded;
  Photometer(double r, std::array<double,3> x);
};

Photometer::Photometer(double r, std::array<double,3> x)
{
  radius = r;
  for (int i = 0; i < 3; i++) {
    position[i] = x[i];
  }
}
