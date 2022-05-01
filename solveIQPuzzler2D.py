#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun May  1 13:58:05 2022

@author: larat
"""
import minos, dlx
import numpy as np
from solvePentaminos import findAllPositions,encode

minos5 = {
    'F' : [1,6,7,8,12],
    'I' : [0,1,2,3, 4],
    'L' : [0,1,2,3, 6],
    'N' : [1,2,3,6, 7],
    'P' : [0,1,2,6, 7],
    'T' : [0,6,7,8,12],
    'U' : [0,1,2,6, 8],
    'V' : [0,1,2,6,12],
    'W' : [1,2,6,7,12],
    'X' : [1,6,7,8,13],
    'Y' : [0,1,2,3, 7],
    'Z' : [2,6,7,8,12]
 }

minos4 = {
    'i' : [0,1,2,3],
    'l' : [0,1,2,5],
    'n' : [1,2,5,6],
    'o' : [0,1,5,6],
    't' : [0,1,2,6]
}

minos3 = {
    '<' : [0,1,4],
    '|' : [0,1,2]
}

minos2 = {'-':[0,1]}

minos1 = {'.':[0]}

mySet = {}
mySet['human'] = [
    minos5['F'],
    minos5['L'],
    minos5['N'],
    minos5['P'],
    minos5['U'],
    minos5['V'],
    minos5['W'],
    minos5['Y'],
    minos4['l'],
    minos4['n'],
    minos4['t'],
    minos3['<']
 ]
mySet['typeToLetter'] = 'FLNPUVWYlnt<'
mySet['letterToType'] = {letter : i for i,letter in enumerate(mySet['typeToLetter'])}
mySet['base'] = []
mySet['orientations'] = []
mySet['types'] = []

setSize = 0
counter = 0
for mino in mySet['human']: 
    setSize += len(mino)
    coords  = minos.toCoordinates(np.array(mino)[...,np.newaxis])
    orientations = minos.getAllOrientations(coords)
    mySet['base'].append(orientations[0])
    mySet['orientations'] += orientations
    mySet['types'] += [counter for i in range(len(orientations))]
    counter += 1
print(setSize)
grid = (5,11)
nCells = grid[0]*grid[1]
assert nCells == setSize
X,Y = findAllPositions(mySet,grid)
 
counter = 0
file = open('SolutionsIQPuzzler2D_Grid%02ix%02i.txt'%(grid[0],grid[1]),'w')
for solution in dlx.solve(X,Y):
    counter += 1
    if counter % 1000 == 0:
        print("%06i solution"%counter)    
    sol_mat = np.zeros(nCells,dtype=int)
    for position in solution:
        piece = Y[position]
        sol_mat[piece[:-1]] = piece[-1]-nCells+1
    file.write(encode(sol_mat)+'\n')
    # plt.figure()
    # plt.pcolor(sol_mat.reshape(grid),edgecolor='k',linewidth=2,cmap=cmap,norm=norm)
    # plt.show()
file.close()

