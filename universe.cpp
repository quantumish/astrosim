#include "tools.cpp"

class Universe
{
public:
  int ticks = 0; // I don't want to disturb the 'nop'... TODO: check if this is bad practice
  std::vector<Matter> matter;
  std::vector<Star> stars;
  std::vector<Photometer> photometers;
  std::vector<Force<Matter, Matter>> forces1;
  std::vector<Force<Matter, Star>> forces2;
  std::vector<Force<Star, Matter>> forces3;
  std::vector<Force<Star, Star>> forces4;
  void update_matter(Matter* obj);
  void add_matter(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a);
  void add_star(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a, double L);
  void add_photometer(double r, Eigen::Vector3d x);
  void check_ray(Photon photon);
  void advance();
  Universe();
};

Universe::Universe()
{
  __asm__("nop"); // Now I can claim parts of this were written in assembly :P
}

template <class T1, class T2>
void calculate_gravity(T1* source, T2* target, Force<T1, T2>* force)
{
  double distance = sqrt(pow(target->position[0] - source->position[0],2)+pow(target->position[1] - source->position[1],2))+pow(target->position[2] - source->position[2],2);
  double magnitude = GRAV_CONST * ((target->mass * source->mass)/distance); // Calculate the magnitude of the force between the objects
  Eigen::Vector3d F = (source->position - target->position).normalized() * magnitude; // Express as vector
  force->components = F;
}

void Universe::add_matter(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a)
{
  matter.emplace_back(m,r,x,v,a);
  for (int i = 0; i < matter.size()-1; i++) {
    Force<Matter, Matter> grav1 (&matter[matter.size()-1], &matter[i], {0,0,0});
    forces1.push_back(grav1);
    calculate_gravity <Matter, Matter> (&matter[matter.size()-1], &matter[i], &forces1[forces1.size()-1]);
    Force<Matter, Matter> grav2 (&matter[i], &matter[matter.size()-1], {0,0,0});
    forces1.push_back(grav2);
    calculate_gravity <Matter, Matter>(&matter[i], &matter[matter.size()-1], &forces1[forces1.size()-1]);
  }
  for (int i = 0; i < stars.size(); i++) {
    Force<Matter, Star> grav1 (&matter[matter.size()-1], &stars[i], {0,0,0});
    forces2.push_back(grav1);
    calculate_gravity<Matter, Star> (&matter[matter.size()-1], &stars[i], &forces2[forces2.size()-1]);
    Force<Star, Matter> grav2 (&stars[i], &matter[matter.size()-1], {0,0,0});
    forces3.push_back(grav2);
    calculate_gravity<Star, Matter> (&stars[i], &matter[matter.size()-1], &forces3[forces3.size()-1]);
  }
}

void Universe::add_star(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a, double L)
{
  stars.emplace_back(m,r,x,v,a,L);
  for (int i = 0; i < matter.size(); i++) {
    Force<Star, Matter> grav1 (&stars[stars.size()-1], &matter[i], {0,0,0});
    forces3.push_back(grav1);
    calculate_gravity<Star, Matter> (&stars[stars.size()-1], &matter[i], &forces3[forces3.size()-1]);
    Force<Matter, Star> grav2 (&matter[i], &stars[stars.size()-1], {0,0,0});
    forces2.push_back(grav2);
    calculate_gravity<Matter, Star> (&matter[i], &stars[stars.size()-1], &forces2[forces2.size()-1]);
  }
  for (int i = 0; i < stars.size()-1; i++) {
    Force<Star, Star> grav1 (&stars[stars.size()-1], &stars[i], {0,0,0});
    forces4.push_back(grav1);
    calculate_gravity<Star, Star> (&stars[stars.size()-1], &stars[i], &forces4[forces4.size()-1]);
    Force<Star, Star> grav2 (&stars[i], &stars[stars.size()-1],{0,0,0});
    forces4.push_back(grav2);
    calculate_gravity<Star, Star> (&stars[i], &stars[stars.size()-1], &forces4[forces4.size()-1]);
  }
}

void Universe::add_photometer(double r, Eigen::Vector3d x)
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
  for (int i = 0; i < forces1.size(); i++) {
    forces1[i].target->net_force.components += forces1[i].components;
    calculate_gravity<Matter, Matter> (forces1[i].source, forces1[i].target, &forces1[i]);
  }
  for (int i = 0; i < forces2.size(); i++) {
    forces2[i].target->net_force.components += forces2[i].components;
    calculate_gravity<Matter, Star> (forces2[i].source, forces2[i].target, &forces2[i]);
  }
  for (int i = 0; i < forces3.size(); i++) {
    forces3[i].target->net_force.components += forces3[i].components;
    calculate_gravity<Star, Matter> (forces3[i].source, forces3[i].target, &forces3[i]);
  }
  for (int i = 0; i < forces4.size(); i++) {
    forces4[i].target->net_force.components += forces4[i].components;
    calculate_gravity<Star, Star> (forces4[i].source, forces4[i].target, &forces4[i]);
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
  //std::cout << "Position:\n" << matter[0].position.transpose() << "\nVelocity:\n" << matter[0].velocity.transpose() << "\nAcceleration:\n" << matter[0].acceleration.transpose() << "\nNet Force:\n" << matter[0].net_force.source << " " << matter[0].net_force.components.transpose()<< "\n\n";
}

int main()
{
  Universe scene{};
  scene.add_star(7.34*pow(10,30), -696.34*pow(10,6), {0,0,0}, {0,0,0}, {0,0,0}, 0);
  scene.add_matter(7.34*pow(10,20), pow(10,6), {10,0,0}, {0,0,0}, {0,0,0});
  for (int i = 0; i < 5; i++) {
    scene.advance();
  }
}

#ifdef PYTHON
PYBIND11_MODULE(astrosim, m) {
  m.doc() = "Simulate exoplanets with C++.";

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
    .def(py::init<>())
    .def("add_matter", &Universe::add_matter, py::arg("m"), py::arg("r"), py::arg("x"), py::arg("v"), py::arg("a"))
    .def("add_star", &Universe::add_star, py::arg("m"), py::arg("r"), py::arg("x"), py::arg("v"), py::arg("a"), py::arg("L"))
    .def("add_photometer", &Universe::add_photometer, py::arg("r"), py::arg("x"))
    .def("advance", &Universe::advance)
    .def_readonly("ticks", &Universe::ticks)
    .def_readonly("matter", &Universe::matter)
    .def_readonly("stars", &Universe::stars)
    .def_readonly("photometers", &Universe::photometers);
}
#endif
