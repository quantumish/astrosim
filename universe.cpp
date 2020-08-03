#include <Eigen/Dense>

#ifdef PYTHON
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
namespace py = pybind11;
#endif

#include "../nbody/nbody.hpp"

// Mathematical constants
#define PI 3.14159265
#define PHI 1.6180339887

// Universal constants
#define WEIN_PROP_CONST (2.897771955 * pow(10, -3))
#define BOLTZMANN_CONST (1.38064852 * pow(10, -23))
#define COULOMB_CONST (8.9876 * pow(10, 9))
#define ELEMENTARY_CHARGE (1.60217653 * pow(10, -19))
#define AVG_NUCLEON_RADIUS (1.3 * pow(10,-12))
#define PLANCK_CONST (6.62607015 * pow(10, -34))
#define GRAV_CONST (6.674 * pow(10,-11))
#define LIGHTSPEED 299792458

// Constants relevant to the simulation
#define LIGHT_FRAC (pow(10, -60)) // Fraction of rays emitted from star to be simulated.
#define LIGHT_EXPIRE 2// Number of ticks a photon exists for (prevent processor from struggling on photons millions of miles away from important stuff)

#include "matter.cpp"
#include "tools.cpp"
#include <SFML/Graphics.hpp>

class Universe : public Sim
{
public:
  // sf::RenderWindow * window;
  std::vector<Matter> matter;
  std::vector<Star> stars;
  std::vector<Photometer> photometers;
  std::vector<Force<Matter, Matter>> forces1;
  std::vector<Force<Matter, Star>> forces2;
  std::vector<Force<Star, Matter>> forces3;
  std::vector<Force<Star, Star>> forces4;
  void update_matter(Matter* obj);
  void update_star(Star* obj);
  void add_matter(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a);
  void add_star(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a, double L);
  void add_photometer(double r, Eigen::Vector3d x);
  void check_ray(Photon photon);
  void draw();
  void advance();
  Universe(double dt, ForceMethod fm, TimeMethod tm);
  //  Universe(sf::RenderWindow * w);
};

Universe::Universe(double dt, ForceMethod fm, TimeMethod tm) : Sim(dt, fm, tm)
{
  __asm__("nop"); // Now I can claim parts of this were written in assembly :P
}

void Universe::add_matter(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a)
{
  matter.emplace_back(m,r,x,v,a);
}

void Universe::add_star(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a, double L)
{
  stars.emplace_back(m,r,x,v,a,L);
}

void Universe::add_photometer(double r, Eigen::Vector3d x)
{
  photometers.emplace_back(r, x);
}

void Universe::check_ray(Photon photon)
{
  for (int i = 0; i < matter.size(); i++) {
    photon.direction *= LIGHTSPEED;
    Eigen::Vector3d L = (photon.position-matter[i].position);
    double a = photon.direction.dot(photon.direction);
    double b = 2 * photon.direction.dot(L);
    double c = L.dot(L) - pow(matter[i].radius,2);
    double discriminant = pow(b,2) - 4*a*c;
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
    if (discriminant == 0) {
      double t0 = -b/(2*a);
      if (t0 >= 0 && t0 <= 1) {
        photometers[i].recorded[t] += 1;
      }
    }
    else if (discriminant > 0) {
      double t0 = (-b+sqrt(discriminant))/(2*a);
      double t1 = (-b-sqrt(discriminant))/(2*a);
      if (t0 >= 0 && t0 <= 1) {
        photometers[i].recorded[t] += 1;
      }
    }
  }
  photon.direction /= LIGHTSPEED;
}

void Universe::advance()
{
  update();
  for (int i = 0; i < photometers.size(); i++) {
    photometers[i].recorded.push_back(0);
  }
  for (int i = 0; i < stars.size(); i++) {
    stars[i].fusion();
    stars[i].radius = sqrt(-GRAV_CONST * stars[i].mass * stars[i].pressure);
    // for (int j = 0; j < stars[i].photons.size(); j++) {
    //   check_ray(stars[i].photons[j]);
    //   stars[i].photons[j].position += stars[i].photons[j].direction * LIGHTSPEED;
    // }
    // if ((ticks+1) % LIGHT_EXPIRE == 0) {
    //   stars[i].kill_light();
    // }
    // stars[i].emit_light();
  }
}

Eigen::Vector2d sfml_pos(Eigen::Vector3d coordinates, sf::RenderWindow* window, int pixel)
{
    std::array<int, 2> fixed_coords;
    fixed_coords[0] = round(coordinates[0]);
    fixed_coords[1] = round(coordinates[1]);
    double pixelLength = pow(10, 2);
    fixed_coords[0] /= pixel;
    fixed_coords[1] /= pixel;
    return Eigen::Vector2d {fixed_coords[0], window->getSize().y-fixed_coords[1]};
}

int sfml_visualize(Universe scene, float pixel)
{
  sf::RenderWindow window(sf::VideoMode(2560, 1600), "AstroSim");
  sf::RenderWindow * windowptr = &window;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }
    window.clear(sf::Color::White);
    scene.advance();
    for (Matter i : scene.matter) {
      sf::CircleShape shape(i.radius / pixel);
      Eigen::Vector2d fixedpos = sfml_pos((i.position.array()-i.radius).matrix(), &window, pixel);
      std::cout << fixedpos[0] << "\n";
      shape.setPosition(fixedpos[0], fixedpos[1]);
      shape.setFillColor(sf::Color::Blue);
      window.draw(shape);
    }
    for (Star i : scene.stars) {
      sf::CircleShape shape(i.radius / pixel);
      Eigen::Vector2d fixedpos = sfml_pos((i.position.array()-i.radius).matrix(), &window, pixel);
      std::cout << fixedpos[0] << "\n";
      shape.setPosition(fixedpos[0], fixedpos[1]);
      shape.setFillColor(sf::Color::Yellow);
      window.draw(shape);
    }
    //std::cout << scene.stars[10020202].radius;
    window.display();
  }
  return EXIT_SUCCESS;
}

int main()
{
  Universe scene{1000, Direct, Leapfrog};
  scene.add_star(pow(10,15), 696.34*pow(10,6), {1000000,1000000,0}, {0,0,0}, {0,0,0}, pow(10,30));
  scene.add_matter(pow(10,9), 696.34*pow(10,1), {1000000,1100000,0}, {30,0,0}, {0,0,0});
  sfml_visualize(scene, 2000);
  // std::cout << "[";
  // for (int i = 0; i < 2500; i++) {
  //   std::cout << scene.photometers[0].recorded[i] << ", ";
  // }
  // std::cout << "]\n";
  // sf::RenderWindow window(sf::VideoMode(2560, 1600), "AstroSim");
  // sf::RenderWindow * windowptr = &window;
  // Universe scene(windowptr);
  // scene.add_star(pow(10,15), 696.34*pow(10,1), {1000000,1000000,0}, {0,0,0}, {0,0,0}, 0);
  // scene.add_matter(pow(10,9), 696.34*pow(10,1), {1000000,1100000,0}, {30,0,0}, {0,0,0});
  // //  scene.add_matter(7.34*pow(10,20), 696.34*pow(10,1), {1000,1000,0}, {0,0,0}, {0,0,0});
  // while (window.isOpen()) {
  //   sf::Event event;
  //   while (window.pollEvent(event)) {
  //     if (event.type == sf::Event::Closed) window.close();
  //   }
  //   //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
  //     window.clear();
  //     scene.draw();
  //     scene.advance();
  //     //}
  //   window.display();
  // }
  // return EXIT_SUCCESS;
}

#ifdef PYTHON
PYBIND11_MODULE(astrosim, m) {
  m.doc() = "Simulate exoplanets with C++.";

  m.def("sfml_visualize", sfml_visualize, py::arg("sim"), py::arg("pixel"));
  py::enum_<ForceMethod>(m, "ForceMethod")
    .value("Direct", ForceMethod::Direct)
    .value("Tree", ForceMethod::Tree)
    .value("FMM", ForceMethod::FMM)
    .value("Mesh", ForceMethod::Mesh)
    .value("P3M", ForceMethod::P3M)
    .export_values();
  py::enum_<TimeMethod>(m, "TimeMethod")
    .value("Euler", TimeMethod::Euler)
    .value("Leapfrog", TimeMethod::Leapfrog)
    .value("Hermite", TimeMethod::Hermite)
    .export_values();
  
  py::class_<Matter>(m, "Matter")
    .def(py::init<double, double, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d>())
    .def_readonly("mass", &Matter::mass)
    .def_readonly("radius", &Matter::radius)
    .def_readonly("position", &Matter::position)
    .def_readonly("velocity", &Matter::velocity)
    .def_readonly("acceleration", &Matter::acceleration);

  py::class_<Star>(m, "Star")
    .def(py::init<double, double, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d, double>())
    .def_readonly("mass", &Star::position)
    .def_readonly("radius", &Star::radius)
    .def_readonly("luminosity", &Star::luminosity)
    .def_readonly("position", &Star::position)
    .def_readonly("velocity", &Star::velocity)
    .def_readonly("acceleration", &Star::acceleration)
    .def_readonly("photons", &Star::photons);

  py::class_<Photometer>(m, "Photometer")
    .def(py::init<double, Eigen::Vector3d>())
    .def_readonly("position", &Photometer::position)
    .def_readonly("radius", &Photometer::radius)
    .def_readonly("recorded", &Photometer::recorded);
  
  py::class_<Universe>(m, "Universe")
    .def(py::init<double, ForceMethod, TimeMethod>())
    .def("add_matter", &Universe::add_matter, py::arg("m"), py::arg("r"), py::arg("x"), py::arg("v"), py::arg("a"))
    .def("add_star", &Universe::add_star, py::arg("m"), py::arg("r"), py::arg("x"), py::arg("v"), py::arg("a"), py::arg("L"))
    .def("add_photometer", &Universe::add_photometer, py::arg("r"), py::arg("x"))
    .def("advance", &Universe::advance)
    .def_readonly("matter", &Universe::matter)
.def_readonly("stars", &Universe::stars)
    .def_readonly("photometers", &Universe::photometers);
}
#endif
