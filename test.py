from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import astrosim

scene = astrosim.Universe()
scene.add_star(7.34*(10**30), 10**6, [0,0,0], [0,0,0], [0,0,0], 10**26)
scene.add_matter(7.34*(10**30), 10**6, [0,0,0], [0,0,0], [0,0,0])
scene.add_matter(5.9*(10**24), 10**2, [0,500000000000000,0], [50,0,0], [0,0,0])
scene.add_photometer(10**2, [0,10000,0])

# for i in range(10):
#     print(i)
#     scene.advance()

# plt.plot(scene.photometers[0].recorded[1:])
# plt.show()

fig = plt.figure()
ax = plt.axes(projection='3d')

x1,x2,x3=[],[],[]
y1,y2,y3=[],[],[]
z1,z2,z3=[],[],[]
for j in range(10000):
   x1.append(scene.matter[0].position[0])
   y1.append(scene.matter[0].position[1])
   z1.append(scene.matter[0].position[2])
   x2.append(scene.matter[1].position[0])
   y2.append(scene.matter[1].position[1])
   z2.append(scene.matter[1].position[2])
   scene.advance()
x3.append(scene.photometers[0].position[0])
y3.append(scene.photometers[0].position[1])
z3.append(scene.photometers[0].position[2])
ax.scatter3D(x2, y2, z2, label="Planet")   
ax.scatter3D(x1, y1, z1, label="Star");
ax.scatter3D(x3, y3, z3, label="Photometer")   

plt.legend()
plt.show()
