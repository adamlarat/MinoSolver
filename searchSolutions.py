#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun May  1 14:56:03 2022

@author: larat
"""
import numpy as np
import pylab as plt
from matplotlib import colors
cmap = plt.cm.rainbow
norm = colors.BoundaryNorm(np.arange(0.5, 12, 1), cmap.N)
plt.close('all')

sol1 = 'LLLLtNNNPPPLWWtttFNNPPUUWWVFFFl<<UnnWVFlllY<UUnnVVVYYYY'
sol2 = 'NYYYYFlll**NNtYFFl****VNttnFF****VNt<nnPP***VVV<<nPPP**'
sol3 = 'llnNYYYYUUUlnnNLLY*U*UlnWNNL*****PPWWNL<****PPPWWL<<***'

grid = (5,11)
solutions = [
    '2666aa7755526caa77b54526cc17bbb442291118334499918888333',
    '2666aa7755526caa77b53526cc17bbb332291118444399918888443',
    '2666aa7733326caa77334426cc17bbb442291118b54599918888555',
    '2666aa7744326caa77444326cc17bbb332291118b53599918888555',
    '2666aa7744326caa77b44326cc17bb4332291118b53599918888555',
    '2666aa7734426caa77b34426cc17bb3342291118b53599918888555',
    '2666aa7755526caa77b53526cc17bb4332291118b44399918888443',
    '2666aa7755526caa77b53526cc17bb3342291118b34499918888344',
    '2666aa7755526caa77b54526cc17bbb442291113384499913338888',
    '2666b88884426cbbb1844426cc771115522977aa13359997aa33355'
    ]

for sol in solutions:
    mat = np.array([int(l,16) for l in sol]).reshape(grid)
    plt.figure()
    plt.pcolor(mat.reshape(grid),edgecolor='k',linewidth=2,cmap=cmap,norm=norm)
    plt.show()

