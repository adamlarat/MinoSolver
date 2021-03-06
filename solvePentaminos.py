#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Apr 30 20:17:03 2022

@author: larat
"""
import minos as p
import dlx as dlx
import numpy as np
import pylab as plt 
plt.close('all')
from matplotlib import colors
cmap = plt.cm.rainbow
norm = colors.BoundaryNorm(np.arange(0.5, 12, 1), cmap.N)

def encode(mesh):
    output = ""
    for cell in mesh:
        if cell<10:
            output += str(cell)
        else:
            output += chr(97+cell-10)
    return output 

def findAllPositions(minosSet,grid):
    nCells = grid[0]*grid[1]
    nMinos = len(minosSet['base'])
    found  = 0
    nOrientations = len(minosSet['orientations'])
    Y = {}
    for i in range(nOrientations):
        yShift = 0
        orientation = minosSet['orientations'][i]
        typ         = minosSet['types'][i]
        position    = np.copy(orientation)  
        while np.max(position[...,0])<grid[0]:
            while np.max(position[...,1])<grid[1]:
                Y[found] = [col for col in np.squeeze(p.toIndices(position,grid[1]))] + [nCells+typ,]
                found           += 1
                position[...,1] += 1
            yShift  += 1
            position = orientation + [yShift,0]
            
    print("Trouvé %i position"%found)
    # Convert to sets for Internet DLX Algorithm
    X = set(range(nCells+nMinos))
    X = {j: set(filter(lambda i: j in Y[i], Y)) for j in X}
    return X,Y


if __name__ == '__main__':
    p5 = p.minos(5)
    #grid = (6,10)
    grid = (3,20)
    nCells = grid[0]*grid[1]
    
    X,Y = findAllPositions(p5,grid)
    
    counter = 0
    file = open('Solutions_Grid%02ix%02i.txt'%(grid[0],grid[1]),'w')
    for solution in dlx.solve(X,Y):
        if counter >10: 
            break
        counter += 1
        print("%04i solution"%counter)    
        sol_mat = np.zeros(nCells,dtype=int)
        for position in solution:
            piece = Y[position]
            sol_mat[piece[:-1]] = piece[-1]-nCells+1
        file.write(encode(sol_mat)+'\n')
        # plt.figure()
        # plt.pcolor(sol_mat.reshape(grid),edgecolor='k',linewidth=2,cmap=cmap,norm=norm)
        # plt.show()
    file.close()
