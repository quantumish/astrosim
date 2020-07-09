#include <vector>
#include <array>
#include <iostream>
#include <cmath>

#include <Eigen/Dense>
#ifdef PYTHON
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
namespace py = pybind11;
#endif
// Mathematical constants
#define PI 3.14159265
#define PHI 1.6180339887

// Universal constants
#define PLANCK_CONST (6.62607015 * pow(10, -34))
#define GRAV_CONST (6.674 * pow(10,-11))
#define LIGHTSPEED 299792458

// Constants relevant to the simulation
#define LIGHT_FRAC (pow(10, -54)) // Fraction of rays emitted from star to be simulated.
#define LIGHT_EXPIRE 1000000// Number of ticks a photon exists for (prevent processor from struggling on photons millions of miles away from important stuff)

class Matter;

struct Force
{
public:
  Eigen::Vector3d components;
  Matter* target;
  Matter* source;
  Force();
  Force(Matter* end, Matter* src, std::array<double, 3> parts);
};

Force::Force()
{
  for (int i = 0; i < 3; i++) {
    components[i] = 0;
  }
  target = nullptr;
  source = nullptr;
}

Force::Force(Matter* end, Matter* src, std::array<double, 3> parts)
{
  target = end;
  source = src;
  for (int i = 0; i < 3; i++) {
    components[i] = parts[i];
  }
};

class Matter
{
public:
  double mass;
  double radius;
  
  Eigen::Vector3d position;
  Eigen::Vector3d velocity;
  Eigen::Vector3d acceleration;
  Force net_force;
  Matter(double m, double r, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a);
};

Matter::Matter(double m, double r, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a)
{
  radius = r;
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
  Eigen::Vector3d blank = {0,0,0};
  net_force.target = this;
  net_force.source = NULL;
  net_force.components = blank;
}

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
  
  Star(double m, double r, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a, double L);
  void emit_light();
  void kill_light();
};

Star::Star(double m, double r, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a, double L) : Matter(m, r, x, v, a)
{
  luminosity = L;
}

void Star::emit_light()
{
  double num_photons = round(LIGHT_FRAC * (luminosity / PLANCK_CONST));
  //  std::cout << "NUM: " << num_photons << " = ("<< luminosity << " / " << PLANCK_CONST << ") * " << LIGHT_FRAC <<"\n";
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

void Star::kill_light()
{
  // Should not be static... as a result changing luminosity not supported.
  double num_photons = round(LIGHT_FRAC * (luminosity / PLANCK_CONST));
  for (int i = 0; i < num_photons; i++) {
    std::cout << i <<"\n";
    photons.erase(photons.begin());
  }
}

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

class Universe
{
public:
  int ticks = 0; // I don't want to disturb the 'nop'... TODO: check if this is bad practice
  std::vector<Matter> matter;
  std::vector<Star> stars;
  std::vector<Photometer> photometers;
  std::vector<Force> forces;
  void update_matter(Matter* obj);
  void add_matter(double m, double r, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a);
  void add_star(double m, double r, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a, double L);
  void add_photometer(double r, std::array<double,3> x);
  void check_ray(Photon photon);
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


void Universe::add_matter(double m, double r, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a)
{
  matter.emplace_back(m,r,x,v,a);
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

void Universe::add_star(double m, double r, std::array<double, 3> x, std::array<double, 3> v, std::array<double, 3> a, double L)
{
  stars.emplace_back(m,r,x,v,a,L);
  //matter.push_back(stars[stars.size()-1]);
  std::array<double, 3> blank = {0,0,0};
  // for (int i = 0; i < matter.size()-1; i++) {
  //   Force grav1 = {&matter[matter.size()-1], &matter[i], blank};
  //   forces.push_back(grav1);
  //   calculate_gravity(&matter[matter.size()-1], &matter[i], &forces[forces.size()-1]);
  //   Force grav2 = {&matter[i], &matter[matter.size()-1], blank};
  //   forces.push_back(grav2);
  //   calculate_gravity(&matter[i], &matter[matter.size()-1], &forces[forces.size()-1]);
  // }
}

void Universe::add_photometer(double r, std::array<double,3> x)
{
  photometers.emplace_back(r, x);
}

void Universe::update_matter(Matter* obj)
{
  obj->position += obj->velocity;
  obj->velocity += obj->acceleration;
  obj->acceleration = obj->net_force.components / obj->mass; // F = ma so F/m = a
}

void Universe::check_ray(Photon photon)
{
  for (int i = 0; i < matter.size(); i++) {
    photon.direction *= LIGHTSPEED;
    Eigen::Vector3d L = (photon.position-matter[i].position);
    double a = photon.direction.dot(photon.direction);
    double b = 2 * photon.direction.dot(L);
    //    std::cout << photon.direction <<"\n\n vs \n\n";
    double c = L.dot(L) - pow(matter[i].radius,2);
    //    std::cout << c << " = " << L.dot(L) << " - " << pow(matter[i].radius, 2) << "\n";
    double discriminant = pow(b,2) - 4*a*c;
    //    std::cout << discriminant << " = " << b << "^2 - (4 * " << a << " * " << c << ")\n";
    if (discriminant == 0) {
      double t0 = -b/(2*a);
      if (t0 >= 0 && t0 <= 1) {
        photon.position = photon.position + photon.direction * t0; // Don't go through the object
        photon.direction = (photon.position - matter[i].position).normalized() * LIGHTSPEED;// Set new direction
      }
    }
    else if (discriminant > 0) {
      double t0 = (-b+sqrt(discriminant))/(2*a);
      //t1 = (-b-sqrt(discriminant))/(2*a);
      if (t0 >= 0 && t0 <= 1) {
        photon.position = photon.position + photon.direction * t0; // Don't go through the object
        photon.direction = (photon.position - matter[i].position).normalized() * LIGHTSPEED;// Set new direction
      }
    }
    photon.direction /= LIGHTSPEED;
  }
  for (int i = 0; i < photometers.size(); i++) {
    photon.direction *= LIGHTSPEED;
    Eigen::Vector3d L = (photon.position-photometers[i].position);
    double a = photon.direction.dot(photon.direction);
    double b = 2 * photon.direction.dot(L);
    double c = L.dot(L) - pow(photometers[i].radius,2);
    double discriminant = pow(b,2) - 4*a*c;
    //    std::cout << photon.direction << "\n\n\n\n";
    //std::cout << discriminant << " = " << b << "^2 - (4 * " << a << " * " << c << ")\n";
    if (discriminant == 0) {
      double t0 = -b/(2*a);
      if (t0 >= 0 && t0 <= 1) {
        photometers[i].recorded[ticks] += 1;
      }
    }
    else if (discriminant > 0) {
      double t0 = (-b+sqrt(discriminant))/(2*a);
      double t1 = (-b-sqrt(discriminant))/(2*a);
      if (t0 >= 0 && t0 <= 1) {
        photometers[i].recorded[ticks] += 1;
      }
    }
  }
  photon.direction /= LIGHTSPEED;
}

void Universe::advance()
{
  for (int i = 0; i < photometers.size(); i++) {
    photometers[i].recorded.push_back(0);
  }
  for (int i = 0; i < matter.size(); i++) update_matter(&matter[i]); // Update all Matter objects.
  for (int i = 0; i < forces.size(); i++) {
    forces[i].target->net_force.components += forces[i].components;
    calculate_gravity(forces[i].target, forces[i].source, &forces[i]);
  }
  for (int i = 0; i < stars.size(); i++) {
    for (int j = 0; j < stars[i].photons.size(); j++) {
      check_ray(stars[i].photons[j]);
      stars[i].photons[j].position += stars[i].photons[j].direction * LIGHTSPEED;
    }
    if ((ticks+1) % LIGHT_EXPIRE == 0) {
      stars[i].kill_light();
    }
    stars[i].emit_light();
  }
  ticks++;
  //std::cout << "Position:\n" << matter[0].position.transpose() << "\nVelocity:\n" << matter[0].velocity.transpose() << "\nAcceleration:\n" << matter[0].acceleration.transpose() << "\n\n";
}

int main()
{
  Universe scene{};
  scene.add_star(7.34*pow(10,22), 696.34*pow(10,6), {0,0,0}, {0,0,0}, {0,0,0}, pow(10,26));
  scene.add_matter(7.34*pow(10,20), pow(10,6), {0,10,0}, {0,0,0}, {0,0,0});
  scene.add_photometer(100, {0,100,0});

  for (int i = 0; i < 5; i++) {
    scene.advance();
  }
  for (int i = 0; i < scene.photometers[0].recorded.size(); i++) {
    std::cout << scene.photometers[0].recorded[i] << "  " << scene.ticks <<"\n";
  }
}

#ifdef PYTHON
PYBIND11_MODULE(astrosim, m) {
  m.doc() = "Simulate exoplanets with C++.";

  py::class_<Matter>(m, "Matter")
    .def(py::init<double, double, std::array<double, 3>, std::array<double, 3>, std::array<double, 3>>())
    .def_readonly("mass", &Matter::mass)
    .def_readonly("radius", &Matter::radius)
    .def_readonly("position", &Matter::position)
    .def_readonly("velocity", &Matter::velocity)
    .def_readonly("acceleration", &Matter::acceleration);

  py::class_<Star>(m, "Star")
    .def(py::init<double, double, std::array<double, 3>, std::array<double, 3>, std::array<double, 3>, double>())
    .def_readonly("mass", &Star::position)
    .def_readonly("radius", &Star::radius)
    .def_readonly("luminosity", &Star::luminosity)
    .def_readonly("position", &Star::position)
    .def_readonly("velocity", &Star::velocity)
    .def_readonly("acceleration", &Star::acceleration)
    .def_readonly("photons", &Star::photons);

  py::class_<Photometer>(m, "Photometer")
    .def(py::init<double, std::array<double,3>>())
    .def_readonly("position", &Photometer::position)
    .def_readonly("radius", &Photometer::radius)
    .def_readonly("recorded", &Photometer::recorded);
  
  py::class_<Universe>(m, "Universe")
    .def(py::init<>())
    .def("add_matter", &Universe::add_matter, py::arg("m"), py::arg("r"), py::arg("x"), py::arg("v"), py::arg("a"))
    .def("add_star", &Universe::add_star, py::arg("m"), py::arg("r"), py::arg("x"), py::arg("v"), py::arg("a"), py::arg("L"))
    .def("add_photometer", &Universe::add_photometer, py::arg("r"), py::arg("x"))
    .def("advance", &Universe::advance)
    .def_readonly("ticks", &Universe::ticks)
    .def_readonly("matter", &Universe::matter)
    .def_readonly("stars", &Universe::stars)
    .def_readonly("forces", &Universe::forces)
    .def_readonly("photometers", &Universe::photometers);
}
#endif
