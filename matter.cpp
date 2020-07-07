#include <vector>
#include <array>
#include <iostream>
#include <cmath>

#include <Eigen/Dense>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
namespace py = pybind11;

// Mathematical constants
#define PI 3.14159265
#define PHI 1.6180339887

// Universal constants
#define PLANCK_CONST (6.62607015 * pow(10, -34))
#define GRAV_CONST (6.674 * pow(10,-11))
#define LIGHTSPEED 299792458

// Constants relevant to the simulation
#define LIGHT_FRAC (pow(10, -59)) // Fraction of rays emitted from star to be simulated.

class Matter
{
public:
  double mass;
  double radius;
  
  Eigen::Vector3d position;
  Eigen::Vector3d velocity;
  Eigen::Vector3d acceleration;
  Matter(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a);
};

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
  target = end;
  source = src;
  for (int i = 0; i < 3; i++) {
    components[i] = parts[i];
  }
};

class Photon
{
public:
  Eigen::Vector3d position;
  Eigen::Vector3d direction; // Unit vector in correct direction
  Photon(Eigen::Vector3d x, Eigen::Vector3d d);
};

Photon::Photon(Eigen::Vector3d x, Eigen::Vector3d d)
{
  position = x;
  direction = d;
}

class Star : public Matter
{
public:
  double luminosity;
  std::vector<Photon> photons; // TODO: Rethink star managing its photons
  
  Star(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a, double L);
  void emit_light();
};

Star::Star(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a, double L) : Matter(m, x, v, a)
{
  luminosity = L;
}

void Star::emit_light()
{
  double num_photons = round(LIGHT_FRAC * (luminosity / PLANCK_CONST));
  std::cout << "NUM: " << num_photons << " = ("<< luminosity << " / " << PLANCK_CONST << ") * " << LIGHT_FRAC <<"\n";
  // Very useful: https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere
  for (int i = 0; (double) i < num_photons; i++) {
    Eigen::Vector3d point;
    point[1] = 1 - (i / (float)(num_photons - 1)) * 2;
    radius = sqrt(1 - point[1] * point[1]);
      
    double theta = PHI * i;
      
    point[0] = cos(theta) * radius;
    point[2] = sin(theta) * radius;
      
    photons.emplace_back(position, point);
  }
}

class Universe
{
public:
  std::vector<Matter> matter;
  std::vector<Star> stars;
  std::vector<Force> forces;
  void update_matter(Matter* obj);
  void add_matter(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a);
  void add_star(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a, double L);
  void advance();
  Universe();
};

Universe::Universe()
{
  __asm__("nop"); // Now I can claim parts of this were written in assembly :P
}

void calculate_gravity(Matter* target, Matter* source, Force* force)
{
  double distance = sqrt(pow(target->position[0] - source->position[0],2)+pow(target->position[1] - source->position[1],2))+pow(target->position[2] - source->position[2],2);
  double magnitude = GRAV_CONST * ((target->mass * source->mass)/distance); // Calculate the magnitude of the force between the objects
  Eigen::Vector3d F = (source->position - target->position).normalized() * magnitude; // Express as vector
  force->components = F;
}


void Universe::add_matter(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a)
{
  matter.emplace_back(m,x,v,a);
  std::array<double, 3> blank = {0,0,0};
  for (int i = 0; i < matter.size()-1; i++) {
    Force grav1 = {&matter[matter.size()-1], &matter[i], blank};
    forces.push_back(grav1);
    calculate_gravity(&matter[matter.size()-1], &matter[i], &forces[forces.size()-1]);
    Force grav2 = {&matter[i], &matter[matter.size()-1], blank};
    forces.push_back(grav2);
    calculate_gravity(&matter[i], &matter[matter.size()-1], &forces[forces.size()-1]);
  }
}

void Universe::add_star(double m, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a, double L)
{
  stars.emplace_back(m,x,v,a,L);
  matter.push_back(stars[stars.size()-1]);
  std::array<double, 3> blank = {0,0,0};
  for (int i = 0; i < matter.size()-1; i++) {
    Force grav1 = {&matter[matter.size()-1], &matter[i], blank};
    forces.push_back(grav1);
    calculate_gravity(&matter[matter.size()-1], &matter[i], &forces[forces.size()-1]);
    Force grav2 = {&matter[i], &matter[matter.size()-1], blank};
    forces.push_back(grav2);
    calculate_gravity(&matter[i], &matter[matter.size()-1], &forces[forces.size()-1]);
  }
}

void Universe::update_matter(Matter* obj)
{
  obj->position += obj->velocity;
  obj->velocity += obj->acceleration;
}

void Universe::advance()
{
  for (int i = 0; i < matter.size(); i++) update_matter(&matter[i]); // Update all Matter objects.
  for (int i = 0; i < forces.size(); i++) {
    forces[i].target->acceleration = forces[i].components / forces[i].target->mass;  // F = ma so F/m = a
    calculate_gravity(forces[i].target, forces[i].source, &forces[i]);
  }
  for (int i = 0; i < stars.size(); i++) {
    for (int j = 0; j < stars[i].photons.size(); j++) {
      stars[i].photons[j].position += stars[i].photons[j].direction * LIGHTSPEED;
    }
    stars[i].emit_light();
  }
  //  std::cout << "Position:\n" << matter[0].position.transpose() << "\nVelocity:\n" << matter[0].velocity.transpose() << "\nAcceleration:\n" << matter[0].acceleration.transpose() << "\n\n";
}

int main()
{
  Star star (pow(10,30), {0,0,0}, {0,0,0}, {0,0,0}, pow(10,26));
  //star.emit_light();
  Matter* test = &star;
  
  // Universe scene{};
  // scene.add_matter(7.34*pow(10,22), {0,0,0}, {0,10000,0}, {0,0,0});
  // scene.add_matter(7.34*pow(10,24), {100000,0,0}, {0,0,0}, {0,0,0});
  // for (int i = 0; i < 10; i++) {
  //   scene.advance();
  // }
}

PYBIND11_MODULE(astrosim, m) {
  m.doc() = "Simulate exoplanets with C++.";

  py::class_<Matter>(m, "Matter")
    .def(py::init<double, std::array<double, 3>, std::array<double, 3>, std::array<double, 3>>())
    .def_readonly("mass", &Matter::position)
    .def_readonly("position", &Matter::position)
    .def_readonly("velocity", &Matter::velocity)
    .def_readonly("acceleration", &Matter::acceleration);

  py::class_<Star>(m, "Star")
    .def(py::init<double, std::array<double, 3>, std::array<double, 3>, std::array<double, 3>, double>())
    .def_readonly("mass", &Matter::position)
    .def_readonly("luminosity", &Matter::position)
    .def_readonly("position", &Star::position)
    .def_readonly("velocity", &Star::velocity)
    .def_readonly("acceleration", &Star::acceleration)
    .def_readonly("photons", &Star::photons);
  
  py::class_<Universe>(m, "Universe")
    .def(py::init<>())
    .def("add_matter", &Universe::add_matter, py::arg("m"), py::arg("x"), py::arg("v"), py::arg("a"))
    .def("add_star", &Universe::add_star, py::arg("m"), py::arg("x"), py::arg("v"), py::arg("a"), py::arg("L"))
    .def("advance", &Universe::advance)
    .def_readonly("matter", &Universe::matter)
    .def_readonly("stars", &Universe::stars);
}
