#!/usr/bin/python3
from collections import defaultdict
import os
from csv import reader

import matplotlib as mpl
mpl.rcParams.update({"font.size": 14})
mpl.use("Agg")
import matplotlib.pyplot as plt
import argparse
from matplotlib.ticker import MaxNLocator

graphs = ["gnutella", "wikivote", "hepth", "bitcoin", "enron", "hepph"]
data = "./savedata"
files = os.listdir(data)
files = [os.path.join(data, f) for f in files]

def plot_distribution(data, xlabel='', ylabel='', title='', xlog=True, ylog=True, showLine=False, intAxis=False, filename=""):
  counts = {}
  for item in data:
      if item not in counts:
          counts[item] = 0
      counts[item] += 1
  counts = sorted(counts.items())
  fig = plt.figure()
  ax = fig.add_subplot(111)
  ax.scatter([k for (k, v) in counts], [v for (k, v) in counts])
  if(len(counts) < 20):  # for tiny graph
      showLine = True
  if showLine == True:
      ax.plot([k for (k, v) in counts], [v for (k, v) in counts])
  if xlog == True:
      ax.set_xscale('log')
  if ylog == True:
      ax.set_yscale('log')
  if intAxis == True:
      gca = fig.gca()
      gca.xaxis.set_major_locator(MaxNLocator(integer=True))
  ax.set_xlabel(xlabel)
  ax.set_ylabel(ylabel)
  # plt.title(title)
  if filename != "":
    fig.savefig(filename, bbox_inches="tight")
  plt.close(fig)

def plotConnectedComponents(file, graph):
  components = defaultdict(int)
  with open(file) as f:
    f.readline()
    r = reader(f)
    for node, comp_id in r:
      comp_id = int(float(comp_id))
      components[comp_id] += 1
  cc_sizes = list(components.values())
  plot_distribution(cc_sizes, xlabel='Weakly connected component size',
                    ylabel='Count', title='Connected component size distributions',
                    filename="../figs/graphBLAS/connect-comp-{}.png".format(graph))


def plotClusteringCoeff(file, graph):
  clustering = []
  with open(file) as f:
    f.readline()
    r = reader(f)
    for node, coef in r:
      coef = float(coef)
      clustering.append(coef)
  plot_distribution(clustering, xlabel='Clustering coefficient',
                  ylabel='Number of vertices', title='Clustering coefficient distributions',
                  xlog=False, ylog=True, showLine=False,
                  filename="../figs/graphBLAS/cluster-coeff-{}.png".format(graph))

def plotDegrees(file, graph):
  degrees = []
  with open(file) as f:
    f.readline()
    r = reader(f)
    for node, deg in r:
      deg = int(float(deg))
      if deg == 0:
        break
      degrees.append(deg)
  plot_distribution(degrees, xlabel="Degree ($k$)",
                    ylabel="Number of nodes with degree $k$ ($N_k$)", title="Degree distributions for {}".format(graph),
                    filename="../figs/graphBLAS/degree-distrib-{}.png".format(graph))


def plotShortestPaths(file, graph):
  dist_cnts = []
  with open(file) as f:
    i = 1
    for dist_cnt in f.readlines():
      dist_cnt = int(dist_cnt)
      dist_cnts += [i]*dist_cnt
      i += 1

  plot_distribution(dist_cnts, xlabel='Shortest path lengths (hops)', 
                  ylabel='Number of paths', title='Shortest path lengths distributions',
                      xlog=False, ylog=False, showLine=True, intAxis=True,
                    filename="../figs/graphBLAS/sssp-{}.png".format(graph))


for graph in graphs:
  rel = [f for f in files if graph in f]
  for file in rel:
    if "Degrees" in file:
      plotDegrees(file, graph)
    elif "ConnectedComponents" in file:
      plotConnectedComponents(file, graph)
    elif "ClusteringCoeff" in file:
      plotClusteringCoeff(file, graph)
    elif "sp_dist" in file:
      plotShortestPaths(file, graph)