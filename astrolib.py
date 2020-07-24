import astrosim
import matplotlib.pyplot as plt
import math

def blackbody_curve(star, start=1e-6, end=1, step=0.01):
    sigma = 5.670373
    temp = star.luminosity / (4 * math.pi * star.radius**2 * sigma)
    wavelength = start
    vals = []
    waves = []
    c = 299792458
    h = 6.62607004 * 10**-34
    k_b = 1.38064852 * 10**23
    epsilon = 1e-6
    while(wavelength < end):
        waves.append(wavelength)
        print(wavelength)
        print(h*c)
#        print(math.exp((h*c)/(wavelength*k_b*temp)+epsilon))
        vals.append((2*h*c)/(wavelength**5) * 1/(math.exp((h*c)/(wavelength*k_b*temp)+epsilon)-1))
        wavelength += step
    return [waves, vals]
