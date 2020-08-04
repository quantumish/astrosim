# :dizzy: astrosim
> Astronomy simulation program written in C++.

## About
AstroSim is a work in progress astronomical simulation program for simulating exoplanets and star systems. 

## :telescope: Observations
A mini python companion library `astrolib` is being developed to aid in visualizing the simulation through matplotlib graphs. This will help with
generating figures like blackbody curves, Hertzbrung-Russel diagrams, radial velocity curves, and potentially more in-depth figures relating to a star's
fusion.

## :earth_americas: Planets
Planets are simply bodies in an N-body simulation that happen to deflect light for now, but in the future more complex behavior like proper collisions, 
materials/densities, or simplistic atmospheres may be present.

## :star: Stars
AstroSim will attempt to double as a stellar evolution simulation by modelling the stars as realistically as possible, simulating processes such as fusion 
supplying the star with enough energy to radiate, hydrostatic forces balancing gravitational forces and pressure, and the life cycle of main-sequence stars.
As many of the details concerning these features are challenging to work out properly, progress may be slow.
