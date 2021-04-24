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
gb_files = [os.path.join(gb_data, f) for f in gb_files if "shortestpath_times" in f]

cu_data = "./nvGraph/graph_results"
cu_files = os.listdir(cu_data)
cu_files = [os.path.join(cu_data, f) for f in cu_files if "SSSP_time" in f]


def load_gb_data(file):
  data = []
  with open(file) as f:
    f.readline()
    f.readline()
    r = reader(f)
    for node, time in r:
      time = float(time)
      data.append(time)
  return data

def load_cu_data(file):
  data = []
  with open(file) as f:
    for d in f.readlines():
      d = int(d)
      # data is in # of microseconds
      data.append(d / 1000000)
  return data

def plotShortestPathTimes(graph):
  fig, ax = plt.subplots()
  plt.tight_layout()
  fig.set_size_inches(5,3)
  bp = ax.boxplot([gb_data[graph], cu_data[graph]], labels=["graphBLAS", "nvGraph"], vert=False, showfliers=False, patch_artist=True)

  colors = ['lightgreen', 'lightblue']
  for patch, color in zip(bp['boxes'], colors):
      patch.set_facecolor(color)

  ax.set_xlabel("Iteration Time (s)")
  # ax.set_xscale ('log')
  # ax.set_xlabel("")
  # ax.legend(bbox_to_anchor=(1,.6), loc="right", columnspacing=1)
  plt.savefig("./figs/shortestpath_iter_times/sp-times-{}.png".format(graph), bbox_inches="tight")
  plt.close(fig)

# list of times in seconds
gb_data = {}
cu_data = {}

for file in cu_files:
  if "gnutella" in file:
    cu_data["gnutella"] = load_cu_data(file)
  elif "wiki" in file:
    cu_data["wikivote"] = load_cu_data(file)
  elif "hepth" in file:
    cu_data["hepth"] = load_cu_data(file)
  elif "bitcoin" in file:
    cu_data["bitcoin"] = load_cu_data(file)
  elif "email" in file:
    cu_data["enron"] = load_cu_data(file)
  elif "hepph":
    cu_data["hepph"] = load_cu_data(file)

for file in gb_files:
  if "gnutella" in file:
    gb_data["gnutella"] = load_gb_data(file)
  elif "wikivote" in file:
    gb_data["wikivote"] = load_gb_data(file)
  elif "hepth" in file:
    gb_data["hepth"] = load_gb_data(file)
  elif "bitcoin" in file:
    gb_data["bitcoin"] = load_gb_data(file)
  elif "enron" in file or "email" in file:
    gb_data["enron"] = load_gb_data(file)
  elif "hepph":
    gb_data["hepph"] = load_gb_data(file)


for graph in graphs:
  plotShortestPathTimes(graph)