from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
import astrosim

scene = astrosim.Universe()
scene.add_matter(7.34*(10**24), [100000,0,0], [0,0,0], [0,0,0])
scene.add_matter(7.34*(10**24), [0,0,0], [0,0,0], [0,0,0])

for i in range(10):
    print(scene.matter[0].position[0][0])
    scene.advance()
