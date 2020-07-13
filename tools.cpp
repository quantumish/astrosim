#include <vector>
#include <array>
#include <Eigen/Dense>

#include "matter.cpp"

class Photometer
{
public:
  double radius;
  Eigen::Vector3d position;
  std::vector<double> recorded;
  Photometer(double r, Eigen::Vector3d x);
};

Photometer::Photometer(double r, Eigen::Vector3d x)
  :radius{r}, position{x}
{
    if (radius <= 0) throw ValueError("Radius of photometer is not positive (Photometer.radius <= 0).");
}
