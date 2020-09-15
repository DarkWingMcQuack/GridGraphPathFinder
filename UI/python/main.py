#! /usr/bin/env python
import argparse
from model import Model
from controller import Controller


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='GirdGraph UI')
    parser.add_argument('--graph', '-g', required=True, type=str)
    parser.add_argument('--selection', '-s', required=False, type=str)
    args = parser.parse_args()

    m = Model(args.graph, args.selection)
    c = Controller(m)
    c.run()
