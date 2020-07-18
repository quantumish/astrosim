#include <vector>
#include <array>
#include <iostream>
#include <cmath>

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
  assert (radius > 0);
  assert (mass >= 0);
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
  assert (luminosity >= 0);
}

void Star::emit_light()
{
  double num_photons = round(LIGHT_FRAC * (luminosity / PLANCK_CONST));
  // Very useful: https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere
  // std::cout << "Emitting " << num_photons << " photons\n";
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
