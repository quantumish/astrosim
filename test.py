from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import astrosim

scene = astrosim.Universe()
scene.add_matter(7.34*(10**20), [10000000,0,0], [0,10,0], [0,0,0])
scene.add_matter(5.19*(10**25), [0,0,0], [0,0,0], [0,0,0])
scene.add_matter(5.19*(10**19), [10010000,0,0], [0,0,0], [0,0,0])

fig = plt.figure()
ax = plt.axes(projection='3d')

for i in range(3):
    x=[]
    y=[]
    z=[]
    for j in range(10):
        x.append(scene.matter[i].position[0])
        y.append(scene.matter[i].position[1])
        z.append(scene.matter[i].position[2])
        scene.advance()
    ax.scatter3D(x, y, z, label=str(scene.matter[i].mass));
    
# x=[]
# y=[]
# z=[]
# for j in range(1000):
#     x.append(scene.stars[0].position[0])
#     y.append(scene.stars[0].position[1])
#     z.append(scene.stars[0].position[2])
#     scene.advance()
# ax.scatter3D(x, y, z, label="Star");

plt.legend()
plt.show()
