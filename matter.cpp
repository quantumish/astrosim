#include <vector>
#include <array>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>

#define GRAV_CONST (6.674 * pow(10,-11))

class Matter
{
public:
  double mass;
  Eigen::Vector3d position;
  Eigen::Vector3d velocity;
  Eigen::Vector3d acceleration;
  Matter(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a);
};

struct Force
{
public:
  Eigen::Vector3d components;
  Matter* target;
  Matter* source;
  Force(Matter* end, Matter* src, std::array<double, 3> parts);
};

Force::Force(Matter* end, Matter* src, std::array<double, 3> parts)
{
  for (int i = 0; i < 3; i++) {
    components[i] = parts[i];
  }
}

Matter::Matter(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a)
{
  mass = m;
  for (int i = 0; i < 3; i++) {
    position[i] = x[i];
  }
  for (int i = 0; i < 3; i++) {
    velocity[i] = v[i];
  }
  for (int i = 0; i < 3; i++) {
    acceleration[i] = a[i];
  }
}

class Universe
{
  std::vector<Matter> matter;
  std::vector<Force> forces;
  void update_matter(Matter* obj);
public:
  void add_matter(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a);
  void advance();
  Universe();
};

Universe::Universe()
{
  // nop
}

void Universe::add_matter(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a)
{
  matter.emplace_back(m,x,v,a);
  
  forces.emplace_back()
  // TODO: Force stuff
}

void Universe::update_matter(Matter* obj)
{
  obj->position += obj->velocity;
  //  std::cout << objposition << "\n\n";
  obj->velocity += obj->acceleration;
}

void Universe::advance()
{
  for (int i = 0; i < matter.size(); i++) {
    update_matter(&matter[i]);
  }
  for (int i = 0; i < forces.size(); i++) {

  }
  //  std::cout << "Position:\n" << matter[0].position.transpose() << "\nVelocity:\n" << matter[0].velocity.transpose() << "\nAcceleration:\n" << matter[0].acceleration.transpose() << "\n\n";
}

Force calculate_gravity(Matter* source, Matter* target)
{
  double distance = sqrt(pow(target->position[0] - source->position[0],2)+pow(target->position[1] - source->position[1],2))+pow(target->position[2] - source->position[2],2);
  double direction = atan2()
}

int main()
{
  Universe scene{};
  scene.add_matter(7.34*pow(10,22), {0,0,0}, {1,0,0}, {1,0,0});
  for (int i = 0; i < 10; i++) {
    scene.advance();
  }
}
