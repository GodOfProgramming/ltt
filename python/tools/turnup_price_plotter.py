#!/usr/bin/env python3

from argparse import ArgumentParser
import matplotlib
import matplotlib.pyplot as plt # pip3 install matplotlib
import json
import os
import sys

parser = ArgumentParser()
parser.add_argument('-a', '--add', help='Append a value to the json', nargs='*', type=int)
args = parser.parse_args()

indir = os.path.dirname(os.path.realpath(__file__))
infile = 'acnh/turnup_prices.json'
in_path = indir + '/' + infile

data = None
with open(in_path) as file:
    data = file.read()
values = json.loads(data)

if args.add is not None and len(args.add) > 0:
    values.extend(args.add)
    data = json.dumps(values, indent=2)
    with open(in_path, 'w') as file:
        file.write(data)
else:
    outdir = os.environ['MEMTMP']
    outfile = 'turnip_prices.png'
    out_path = outdir + '/' + outfile

    y_axis = values
    num_entries = len(y_axis)
    x_axis = []
    x_axis.extend(range(1, num_entries + 1))

    matplotlib.use('TkAgg')
    plt.plot(x_axis, y_axis)
    plt.ylabel('Prices')
    plt.xlabel('Day Number')

    plt.savefig(out_path, bbox_inches="tight")
    plt.show()

