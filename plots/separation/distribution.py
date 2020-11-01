#! /usr/bin/env python

import argparse

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm
from mayavi import mlab
from mpl_toolkits.mplot3d import Axes3D
import csv

def read_csv(path):
    with open(path,'r') as csvfile:
        data = csv.reader(csvfile, delimiter=',')
        tuples = [(int(row[0]), int(row[1]), int(row[2])) for row in data]
        x = [x for x, _, _ in tuples]
        y = [y for _, y, _ in tuples]
        z = [z for _, _, z in tuples]
        return x, y, z


def plot3D(x, y, z):
    fig = plt.figure()
    ax = Axes3D(fig)
    surf = ax.plot_trisurf(x, y, z, cmap=cm.jet, linewidth=0.1)
    fig.colorbar(surf, shrink=0.5, aspect=5)
    plt.show()

def plotBar(x, y, z):
    xy = [f"{x}:{y}" for x, y in zip(x, y)]

    plt.bar(xy, z)
    plt.tight_layout()
    plt.xticks(rotation=45)
    plt.show()



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='distribution plot')
    parser.add_argument('--file', '-f', required=True, type=str)
    args = parser.parse_args()

    path = args.file
    x, y, z = read_csv(path)

    plot3D(x, y, z)
    plotBar(x, y, z)
