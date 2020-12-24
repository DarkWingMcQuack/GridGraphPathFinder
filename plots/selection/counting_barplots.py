#! /usr/bin/env python

import argparse

import matplotlib.pyplot as plt
from collections import Counter

DATA={
        3 : 3,
4 : 21,
5 : 45,
6 : 110,
7 : 165,
8 : 265,
9 : 381,
10 : 446,
11 : 445,
12 : 541,
13 : 539,
14 : 495,
15 : 437,
16 : 389,
17 : 330,
18 : 257,
19 : 210,
20 : 151,
21 : 98,
22 : 68,
23 : 50,
24 : 32,
25 : 23,
26 : 8,
27 : 8,
28 : 1,
29 : 6,
30 : 3,
31 : 2,
33 : 1
}

def plotBar():
    plt.bar(DATA.keys(), DATA.values())
    plt.tight_layout()
    plt.xticks(rotation=45)
    plt.show()


if __name__ == '__main__':
    plotBar()
