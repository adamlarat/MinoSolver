#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May  6 21:00:57 2022

@author: larat
"""

import sys,os
import numpy as np

toColorMap = {
 '1':"\033[48;2;230;137;0mF",
 '2':"\033[48;2;211;8;13mL",
 '3':"\033[48;2;231;107;161mN",
 '4':"\033[48;2;160;251;184mP",
 '5':"\033[48;2;144;185;28mU",
 '6':"\033[48;2;0;133;189mV",
 '7':"\033[48;2;120;29;118mW",
 '8':"\033[48;2;250;230;0mY",
 '9':"\033[48;2;20;61;140ml",
 'a':"\033[48;2;118;29;27mn",
 'b':"\033[48;2;0;133;86mt",
 'c':"\033[48;2;115;190;250m<"
}
toNumberMap = {
 'F':'1',
 'L':'2',
 'N':'3',
 'P':'4',
 'U':'5',
 'V':'6',
 'W':'7',
 'Y':'8',
 'l':'9',
 'n':'a',
 't':'b',
 '<':'c',
}

def toNumbers(chars):
    numbers = ''
    for c in chars: 
        if c in toNumberMap:
            numbers += toNumberMap[c]
        else:
            numbers += '.'
    return numbers

def toColor(numbers):
    chain = ""
    for c in numbers:
        chain += toColorMap[c]
    return chain+"\033[00;0m"

def splitLine(line,gridType,gridSize):
    if gridType.lower() == "grid":
        return [line[i:i+gridSize] for i in range(0,len(line),gridSize)]
    if gridType.lower() == "pyramide":
        lengths = [item for subliste in [i*[i,] for i in range(gridSize,0,-1)] for item in subliste]
        cumul = [(elem,int(sum(lengths[:i]))) for i,elem in enumerate(lengths)]
        lines = [line[i:i+j] for j,i in cumul]
        return [lines[i] for i in range(len(lengths)-1,-1,-1)]
    print("wrong grid type : ",gridType,gridSize)
    exit()

if len(sys.argv) < 3:
    print("You need to provide a pattern")
    print(" Syntax : python findPattern.py filename pattern")
    print("Come back soon!")
    exit()

filename = sys.argv[1]
pattern  = toNumbers(sys.argv[2])
gridTok  = filename.split('_')[-1].split('.')[0].split('x')[0]
gridType = ''.join(filter(str.isalpha, gridTok))
gridSize = int(''.join(filter(str.isdigit, gridTok)))
termOut  = os.popen("cat '"+filename+"' | grep -e "+pattern)
ans      = termOut.readline().strip()
while ans:
    lines = splitLine(ans,gridType,gridSize)
    for line in lines:
        print(toColor(line))
    print("--------------------------------")
    ans = termOut.readline().strip()
