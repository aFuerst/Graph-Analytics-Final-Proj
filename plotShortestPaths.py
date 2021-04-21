#!/usr/bin/python3
from collections import defaultdict
import os
from csv import reader

import matplotlib as mpl
mpl.rcParams.update({'font.size': 14})
mpl.use('Agg')
import matplotlib.pyplot as plt
import argparse

graphs = ["gnutella", "wikivote", "hepth", "bitcoin", "enron", "hepph"]
gb_data = "./graphBLAS/savedata"
gb_files = os.listdir(gb_data)
gb_files = [os.path.join(gb_data, f) for f in gb_files]

def plotShortestPathTimes():
  gb_times = defaultdict(list)
  for file in gb_files:
    if "shortestpath_times" in file:
      graph = file.split("/")[-1].split(".")[0].split("-")[1]

      with open(file) as f:
        f.readline()
        r = reader(f)
        _, tot_time = r.__next__()
        for i, time in r:
          gb_times[graph].append(float(time))

  fig, ax = plt.subplots()
  plt.tight_layout()
  fig.set_size_inches(5,3)
  ax.boxplot(gb_times.values(), labels=list(gb_times.keys()), vert=False)

  ax.set_xlabel("Iteration Time (s)")
  # ax.set_yscale ('log')
  # ax.set_xlabel("")
  # ax.legend(bbox_to_anchor=(1,.6), loc="right", columnspacing=1)
  plt.savefig("./figs/shortestpath.png", bbox_inches="tight")
  plt.close(fig)


plotShortestPathTimes()