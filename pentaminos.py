#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Apr 30 14:43:06 2022

@author: larat
"""
import numpy as np
import pylab as plt 
plt.close('all')

def isCoordinates(pentamino):
    return np.shape(pentamino)[-1] == 2

def isIndices(pentamino):
    return np.shape(pentamino)[-1] == 1

def taille(pentamino):
    return np.shape(pentamino)[-2]
        
def toIndices(pentamino,gridSize=0):
    # Check that pentamino is in coordinates
    assert isCoordinates(pentamino)
    # Get pentamino size
    if gridSize == 0:
        gridSize = taille(pentamino)+1
    # Return indices description
    return (pentamino[...,1]+gridSize*pentamino[...,0])[...,np.newaxis]

def toCoordinates(pentamino,gridSize=0):
    # Check that pentamino is in indices
    assert isIndices(pentamino)
    # Get pentamino size
    if gridSize==0:
        gridSize = taille(pentamino)+1
    # Return coordinate description
    return np.append(pentamino//gridSize,pentamino%gridSize,axis=-1)

def shift(pentamino):
    # Check that pentamino is in coordinates
    assert isCoordinates(pentamino)
    return pentamino - np.amin(pentamino,axis=tuple(range(pentamino.ndim-1)))

def reorder(pentamino):
    if isIndices(pentamino):
        return np.sort(pentamino,axis=-2)
    elif isCoordinates(pentamino):
        return toCoordinates(np.sort(toIndices(pentamino),axis=-2))
    else:
        print("What is that?")
        assert False

def hSymetry(pentamino):
    # Check that pentamino is in coordinates
    assert isCoordinates(pentamino)
    n = taille(pentamino)
    symetric        = n-pentamino
    symetric[...,1] = pentamino[...,1]
    return reorder(shift(symetric))

def vSymetry(pentamino):
    # Check that pentamino is in coordinates
    assert isCoordinates(pentamino)
    n = taille(pentamino)
    symetric        = n-pentamino
    symetric[...,0] = pentamino[...,0]
    return reorder(shift(symetric))

def xSymetry(pentamino):
    # Check that pentamino is in coordinates
    assert isCoordinates(pentamino)
    return reorder(np.roll(pentamino,1,axis=-1))

def neighbors(cell):
    return np.array([[cell+[-1,0]],[cell+[0,-1]],[cell+[0,1]],[cell+[1,0]]])

def getAllTransformations(pentamino):
    transformations = np.array([pentamino])
    for trans in [xSymetry,hSymetry,vSymetry]:
        for penta in transformations:
            newTrans = trans(penta)
            isNew = True
            for oldTrans in transformations: 
                if np.array_equal(oldTrans,newTrans):
                    isNew = False
                    break
            if isNew: 
                transformations = np.append(transformations,[newTrans],axis=0)
    # Sort these guys by increasing order of the sum of indices
    return transformations[np.sum(2**(toIndices(transformations)),axis=(-2,-1)).argsort()]
    
def plot(pentamino,grid=(0,0),tipe=0,Np=1):
    if grid[0]+grid[1] == 0:
        n = taille(pentamino)
        grid = (n,n)
    mat = np.ones(grid)
    color = tipe/float(Np)
    for [y,x] in pentamino:
        mat[y,x] = color
    plt.figure()
    plt.pcolor(mat,edgecolor='k',linewidth=2)
    plt.show()
    return

def pentaminos(n):
    """
        the structure of the returned dictionnary is : 
            'base' is a (Np,n,d) array, where
                * n is the size of the pentaminos
                * Np is the number of pentaminos of size n
                * d is the dimension we are working in. d=2 for the moment.
            'orientations' is a list of all the different rotations and symetries of the pentaminos.
        The pentaminos are always stored in lexicographic order and 
            as high and left as possible in the (n+1)x(n+1) grid. 
    """
    if n==1: 
        return {
            'base': np.array([[[0,0]]]),
            'orientations': np.array([[[0,0]]]),
            'types': np.array([0])
        }
    else:
        pentaminos_m1 = pentaminos(n-1)['base']
        ### pentaminos(n-1) are defined in a nxn grid
        ### shift them in a (n+1)x(n+1) grid 
        pentaminos_m1 += 1
        found = 0
        pentaminos_l = []
        for pm1 in pentaminos_m1:
            pm1_indices = toIndices(pm1,n+1)
            for cell in pm1:
                for newCell in neighbors(cell):
                    # If new cell already exists, continue
                    if toIndices(newCell,n+1) in pm1_indices:
                        continue
                    newPentamino = reorder(shift(np.append(pm1,newCell,axis=-2)))
                    rotations = getAllTransformations(newPentamino)
                    isNew = True
                    for oldPenta in pentaminos_l:
                        if np.array_equal(rotations[0],oldPenta):
                            isNew = False
                            break
                    if isNew:
                        if found == 0: 
                            pentaminos_l = np.array([rotations[0]])
                            orientations_l = rotations
                            types = np.zeros(np.shape(rotations)[0],dtype=int)
                        else: 
                            pentaminos_l = np.append(pentaminos_l,[rotations[0]],axis=0)
                            orientations_l = np.append(orientations_l,rotations,axis=0)
                            types   = np.append(types,found*np.ones(np.shape(rotations)[0],dtype=int))
                        found += 1     
        return {
            'base': pentaminos_l,
            'orientations': orientations_l,
            'types': types
            }
        
if __name__ == '__main__':
    n = 5
    p = pentaminos(n)
    print("Found %i pentaminos with %i cells"%(np.shape(p['base'])[0],n))
    # typ = -1
    # Np = np.shape(p['base'])[0]
    # for i in range(np.shape(p['orientations'])[0]):
    #     if p['types'][i] != typ:
    #         typ = p['types'][i]
    #         counter = 0
    #     plot(p['orientations'][i],tipe=typ,Np=Np)
    #     plt.savefig('%02i_rotation%i.png'%(typ+1,counter+1))
    #     plt.close()
    #     counter += 1
