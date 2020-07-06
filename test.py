from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import astrosim

scene = astrosim.Universe()
scene.add_matter(7.34*(10**20), [3840000000,0,0], [0,100000000,0], [0,0,0])
scene.add_matter(5.19*(10**25), [0,0,0], [0,0,0], [0,0,0])

fig = plt.figure()
ax = plt.axes(projection='3d')

for i in range(2):
    x=[]
    y=[]
    z=[]
    for j in range(1000):
        x.append(scene.matter[i].position[0])
        y.append(scene.matter[i].position[1])
        z.append(scene.matter[i].position[2])
        scene.advance()
    ax.scatter3D(x, y, z, label=str(i));
plt.legend()
plt.show()
