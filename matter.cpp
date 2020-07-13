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
#define LIGHT_EXPIRE 3// Number of ticks a photon exists for (prevent processor from struggling on photons millions of miles away from important stuff)

struct ValueError : public std::exception
{
  const char* error_message;
  ValueError(const char* msg) {
    error_message = msg;
  }
  const char* what() const throw () {
    return error_message;
  }
};

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

class Matter
{
public:
  double mass;
  double radius;
  
  Eigen::Vector3d position;
  Eigen::Vector3d velocity;
  Eigen::Vector3d acceleration;
  Force<Matter, Matter> net_force;
  Matter(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a);
};

Matter::Matter(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a)
  :radius{r}, mass{m}, net_force{this, NULL, {0,0,0}}, position{x}, velocity{v}, acceleration{a}
{
  if (radius <= 0) throw ValueError("Radius of matter is not positive (Matter.radius <= 0).");
  if (mass < 0) throw ValueError("Mass of matter is negative (Matter.mass < 0).");
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

class Star : public Matter
{
public:
  double luminosity;
  std::vector<Photon> photons; // TODO: Rethink star managing its photons
  
  Star(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a, double L);
  void emit_light();
  void kill_light();
};

Star::Star(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a, double L)
  : Matter(m, r, x, v, a), luminosity{L}
{
  if (luminosity < 0) throw ValueError("Luminosity of star is negative (star.luminsoriy < 0).");
}

void Star::emit_light()
{
  double num_photons = round(LIGHT_FRAC * (luminosity / PLANCK_CONST));
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
  std::vector<decltype(photons)::value_type>(photons.begin()+num_photons, photons.end()).swap(photons);

}
