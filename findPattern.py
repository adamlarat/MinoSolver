#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May  6 21:00:57 2022

@author: larat
"""

import sys,os

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

if len(sys.argv) < 3:
    print("You need to provide a pattern")
    print(" Syntax : python findPattern.py filename pattern")
    print("Come back soon!")
    exit()

filename = sys.argv[1]
pattern  = toNumbers(sys.argv[2])
gridLen  = int(filename.split('x')[1].split('.')[0])
termOut  = os.popen("cat '"+filename+"' | grep -e "+pattern)
ans      = termOut.readline().strip()
while ans:
    lines = [ans[i:i+gridLen] for i in range(0,len(ans),gridLen)]
    for line in lines:
        print(toColor(line))
    print("--------------------------------")
    ans = termOut.readline().strip()
