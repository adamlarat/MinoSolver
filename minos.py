#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Apr 30 14:43:06 2022

@author: larat
"""
import numpy as np
import pylab as plt 
plt.close('all')

def isCoordinates(mino):
    return np.shape(mino)[-1] == 2

def isIndices(mino):
    return np.shape(mino)[-1] == 1

def taille(mino):
    return np.shape(mino)[-2]
        
def toIndices(mino,gridSize=0):
    # Check that mino is in coordinates
    assert isCoordinates(mino)
    # Get mino size
    if gridSize == 0:
        gridSize = taille(mino)+1
    # Return indices description
    return (mino[...,1]+gridSize*mino[...,0])[...,np.newaxis]

def toCoordinates(mino,gridSize=0):
    # Check that mino is in indices
    assert isIndices(mino)
    # Get mino size
    if gridSize==0:
        gridSize = taille(mino)+1
    # Return coordinate description
    return np.append(mino//gridSize,mino%gridSize,axis=-1)

def shift(mino):
    # Check that mino is in coordinates
    assert isCoordinates(mino)
    return mino - np.amin(mino,axis=tuple(range(mino.ndim-1)))

def reorder(mino):
    if isIndices(mino):
        return np.sort(mino,axis=-2)
    elif isCoordinates(mino):
        return toCoordinates(np.sort(toIndices(mino),axis=-2))
    else:
        print("What is that?")
        assert False

def hSymetry(mino):
    # Check that mino is in coordinates
    assert isCoordinates(mino)
    n = taille(mino)
    symetric        = n-mino
    symetric[...,1] = mino[...,1]
    return reorder(shift(symetric))

def vSymetry(mino):
    # Check that mino is in coordinates
    assert isCoordinates(mino)
    n = taille(mino)
    symetric        = n-mino
    symetric[...,0] = mino[...,0]
    return reorder(shift(symetric))

def xSymetry(mino):
    # Check that mino is in coordinates
    assert isCoordinates(mino)
    return reorder(np.roll(mino,1,axis=-1))

def neighbors(cell):
    return np.array([[cell+[-1,0]],[cell+[0,-1]],[cell+[0,1]],[cell+[1,0]]])

def getAllOrientations(mino):
    orientations = [mino]
    for trans in [xSymetry,hSymetry,vSymetry]:
        for m in orientations:
            newTrans = trans(m)
            isNew = True
            for oldTrans in orientations: 
                if np.array_equal(oldTrans,newTrans):
                    isNew = False
                    break
            if isNew: 
                orientations.append(newTrans)
    orientations.sort(key=lambda o: np.sum(2**(toIndices(o))))
    # Sort these guys by increasing order of the sum of indices
    return orientations
    #return orientations[np.sum(2**(toIndices(orientations)),axis=(-2,-1)).argsort()]
    
def plot(mino,grid=(0,0),tipe=0,Np=1):
    if grid[0]+grid[1] == 0:
        n = taille(mino)
        grid = (n,n)
    mat = np.ones(grid)
    color = tipe/float(Np)
    for [y,x] in mino:
        mat[y,x] = color
    plt.figure()
    plt.pcolor(mat,edgecolor='k',linewidth=2)
    plt.show()
    return

def minos(n):
    """
        the structure of the returned dictionnary is : 
            'base' is a Np array of (n,d) np-array, where
                * n is the size of the minos
                * Np is the number of n-minos
                * d is the dimension we are working in. d=2 for the moment.
            'orientations' is an array of all the different rotations and symetries of the minos.
            'types' links a given orientation with its 'base' form: 
                'orientations'[i] is a given position of the 'base'['types'[i]] mino
        The minos are always stored in lexicographic order and 
            as high and left as possible in the (n+1)x(n+1) grid. 
    """
    if n==1: 
        return {
            'base': [np.array([[0,0]],dtype=int)],
            'orientations': [np.array([[0,0]],dtype=int)],
            'types': [0]
        }
    else:
        minos_m1 = minos(n-1)['base']
        ### minos(n-1) are defined in a nxn grid
        ### shift them in a (n+1)x(n+1) grid 
        minos_m1 = [mino+1 for mino in minos_m1]
        found   = 0
        minos_l = []
        orientations_l = []
        types = []
        for pm1 in minos_m1:
            pm1_indices = toIndices(pm1,n+1)
            for cell in pm1:
                for newCell in neighbors(cell):
                    # If new cell already exists, continue
                    if toIndices(newCell,n+1) in pm1_indices:
                        continue
                    newMino   = reorder(shift(np.append(pm1,newCell,axis=-2)))
                    rotations = getAllOrientations(newMino)
                    isNew = True
                    for oldPenta in minos_l:
                        if np.array_equal(rotations[0],oldPenta):
                            isNew = False
                            break
                    if isNew:
                        minos_l.append(rotations[0])
                        orientations_l += rotations
                        types += [found for x in range(len(rotations))]
                        found += 1     
        return {
            'base': minos_l,
            'orientations': orientations_l,
            'types': types
            }
        
if __name__ == '__main__':
    n = 5
    p = minos(n)
    print("Found %i minos with %i cells"%(len(p['base']),n))
    # typ = -1
    # Np = len(p['base'])
    # for i in range(len(p['orientations'])):
    #     if p['types'][i] != typ:
    #         typ = p['types'][i]
    #         counter = 0
    #     plot(p['orientations'][i],tipe=typ,Np=Np)
    #     plt.savefig('%02i_rotation%i.png'%(typ+1,counter+1))
    #     plt.close()
    #     counter += 1
