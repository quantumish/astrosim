from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import astrosim

scene = astrosim.Universe()
scene.add_star(9.9*(10**24), 696 * 10**6, [0,0,0], [0,0,0], [0,0,0], 10**0)
scene.add_matter(5.9*(10**24), 6 * 10**6, [0,94*10**9,0], [20000,0,0], [0,0,0])
scene.add_photometer(10**2, [0,10000000,0])

# for i in range(10):
#     # print(i)
#     scene.advance()

# plt.plot(scene.photometers[0].recorded[1:])
# plt.show()

x1.append(scene.forces2[0].components[0])
   y1.append(scene.forces2[0].components[1])
   z1.append(scene.forces2[0].components[2])

fig = plt.figure()
ax = plt.axes(projection='3d')

# x1,x2,x3=[],[],[]
# y1,y2,y3=[],[],[]
# z1,z2,z3=[],[],[]
# for j in range(10000):
#    print(j)
#    x1.append(scene.matter[0].position[0])
#    y1.append(scene.matter[0].position[1])
#    z1.append(scene.matter[0].position[2])
#    x2.append(scene.stars[0].position[0])
#    y2.append(scene.stars[0].position[1])
#    z2.append(scene.stars[0].position[2])
#    scene.advance()
# x3.append(scene.photometers[0].position[0])
# y3.append(scene.photometers[0].position[1])
# z3.append(scene.photometers[0].position[2])

# fixedx1, fixedx2 = [],[]
# fixedy1, fixedy2 = [],[]
# fixedz1, fixedz2 = [],[]

# i = 0
# while (i < len(x1)):
#    if (i % 1000 == 0):
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

plt.legend()
plt.show()
