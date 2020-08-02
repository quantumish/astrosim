from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import astrosim
import math

import astrolib

scene = astrosim.Universe(1, astrosim.Direct, astrosim.Euler)
scene.add_star(10**15, 696 * 10**6, [0,0,0], [0,0,0], [0,0,0], 10**30)
scene.add_matter(10**9, 6 * 10**6, [0,100000, 0], [1,0,0], [0,0,0])

#astrolib.sfml_visualize(scene, 1000)

#scene.advance()

# scene.add_photometer(10**5, [1200000,1000000,0])

# velocity = []
# for i in range(10000):
#    print(i)
#    scene.advance()
#    velocity.append(math.sqrt(scene.stars[0].velocity[0]**2+scene.stars[0].velocity[1]**2+scene.stars[0].velocity[2]**2))

# plt.xlabel("Time (in simulation ticks)")
# plt.ylabel("Magnitude of Velocity (in m/s)")
# plt.title("Radial Velocity of Star")
# plt.plot(velocity)
# plt.show()   
              
# fig = plt.figure()
# ax = plt.axes(projection='3d')

# accel = []
# x1,x2,x3=[],[],[]
# y1,y2,y3=[],[],[]
# z1,z2,z3=[],[],[]
# for j in range(10000):
# #   print(j)
#    x1.append(scene.matter[0].position[0])
#    y1.append(scene.matter[0].position[1])
#    z1.append(scene.matter[0].position[2])
#    x2.append(scene.stars[0].position[0])
#    y2.append(scene.stars[0].position[1])
#    z2.append(scene.stars[0].position[2])
# #   print(accel[j])
#    scene.advance()
# x3.append(scene.photometers[0].position[0])
# y3.append(scene.photometers[0].position[1])
# z3.append(scene.photometers[0].position[2])

# fixedaccel = []
# fixedx1, fixedx2 = [],[]
# fixedy1, fixedy2 = [],[]
# fixedz1, fixedz2 = [],[]

# i = 0
# while (i < len(x1)):
#    if (i % 10 == 0):
#       fixedx1.append(x1[i])
#       fixedx2.append(x2[i])
#       fixedy1.append(y1[i])
#       fixedy2.append(y2[i])
#       fixedz1.append(z1[i])
#       fixedz2.append(z2[i])
#    i+=1

# s = 20*4**1
# ax.scatter3D(fixedx1, fixedy1, fixedz1, label="Planet");
# ax.scatter3D(fixedx2, fixedy2, fixedz2, label="Star", s=s);
# ax.scatter3D(x3, y3, z3, label="Photometer"); 

# plt.legend()
# plt.show()
