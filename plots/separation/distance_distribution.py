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
        tuples = [(int(row[0]), float(row[1])) for row in data]
        x = [x for x, _ in tuples]
        y = [y for _, y in tuples]
        return x, y


def plotBar(x, y):
    x = [str(elem) for elem in x]
    plt.bar(x, y)
    plt.tight_layout()
    plt.xticks(rotation=45)
    plt.show()



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='distance distribution plotter')
    parser.add_argument('--file', '-f', required=True, type=str)
    args = parser.parse_args()

    path = args.file
    x, y = read_csv(path)

    plotBar(x, y)
