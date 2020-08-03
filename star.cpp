class Ring
{
  std::vector<double> contents;
  double temp;
  double pressure;
  bool fusion;
}

class Star : public Matter
{
public:
  double luminosity;
  std::vector<Ring> layers;
  int valid_fusion;

  std::vector<Photon> photons; // TODO: Rethink star managing its photons
  
  Star(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a, double L);
  void fusion();
  void emit_light();
  void kill_light();
};

Star::Star(double m, double r, Eigen::Vector3d x, Eigen::Vector3d v, Eigen::Vector3d a, double L)
  : Matter(m, r, x, v, a), luminosity{L}, core{(0.1*mass/1.007825)*MOLE,0,0,0,0}, shell{(0.9*mass/1.007825)*MOLE,0,0,0,0}, core_radius{0.25*r}
{
  assert (luminosity >= 0);
  assert (mass > 0);
}

void Star::fusion()
{
  valid_fusion = 0;
  for (Ring i : layers) {
    i.fusion = false;
    if (i.content[0] > 0 && 3/2 * BOLTZMANN_CONST * i.temp >= ((pow(ELEMENTARY_CHARGE, 2)*COULOMB_CONST) * (1/(2*AVG_NUCLEON_RADIUS)))) {
      i.fusion = true;
      valid_fusion++;
    }
  }
  double newmass = 0;
  for (Ring i : layers) {
    if (fusion == true) {
      i.contents[0] -= (((luminosity/valid_fusion)/(pow(LIGHTSPEED,2)))/0.028698) * 4;
      i.contents[1] += (((luminosity/valid_fusion)/(pow(LIGHTSPEED,2)))/0.028698);
      newmass += (((i.contents[0]/MOLE) * 1.007825)+((i.contents[1]/MOLE) * 4.002602)) + ((shell[0]/MOLE) * 1.007825);
      std::cout << core[0] << " "<< core[1] << " "<< mass << " " << core_temp << "\n";
    }
  }
  mass = newmass;
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
