from collections import defaultdict
import os
from csv import reader

import matplotlib as mpl
mpl.rcParams.update({"font.size": 14})
mpl.use("Agg")
import matplotlib.pyplot as plt
import argparse

graphs = ["gnutella", "wiki", "hepth", "bitcoin", "email", "HepPh"]
sssp_data = ["graph_results/06_gnutella_SSSP.txt", "graph_results/02_wiki_SSSP.txt", "graph_results/04_hepth_SSSP.txt", "graph_results/03_bitcoin_SSSP.txt", "graph_results/01_email_SSSP.txt", "graph_results/05_ca_HepPh_SSSP.txt"]
data = "./graph_results"
files = os.listdir(data)
files = [os.path.join(data, f) for f in files]

def plot_distribution(data, xlabel='', ylabel='', title='', xlog=True, ylog=True, showLine=False, intAxis=False, filename=""):
  counts = {}
  i = 0
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
  plt.title(title)
  if filename != "":
    fig.savefig(filename, bbox_inches="tight")

def plotSSSP(file, graph):
  SSSP = []
  with open(file) as f:
    for i, short in enumerate(f.readlines()):
      short = int(short)
      SSSP += [i]*short
  plot_distribution(SSSP, xlabel="Shortest Path Lengths ($hops$)",
                    ylabel="Number of Paths", title="Shortest Path Distributions for {}".format(graph),
                    filename="../figs/nvGraph/sssp-{}.png".format(graph),
                    showLine=True, xlog=False, ylog=False)


def plotConnectedComponents(file, graph):
  components = []
  with open(file) as f:
    for comp_size in f.readlines():
      comp_size = int(comp_size)
      components.append(comp_size)
  plot_distribution(components, xlabel='Weakly connected component size',
                    ylabel='Count', title='Connected component size distributions',
                    filename="../figs/nvGraph/connect-comp-{}.png".format(graph))


def plotClusteringCoeff(file, graph):
  clustering = []
  with open(file) as f:
    for coef in f.readlines():
      coef = float(coef)
      clustering.append(coef)
  plot_distribution(clustering, xlabel='Clustering coefficient',
                  ylabel='Number of vertices', title='Clustering coefficient distributions',
                  xlog=False, ylog=True, showLine=False,
                  filename="../figs/nvGraph/cluster-coeff-{}.png".format(graph))

def plotDegrees(file, graph):
  degrees = []
  with open(file) as f:
    for i, deg in enumerate(f.readlines()):
      deg = int(deg)
      degrees += [i]*deg
  plot_distribution(degrees, xlabel="Degree ($k$)",
                    ylabel="Number of nodes with degree $k$ ($N_k$)", title="Degree distributions for {}".format(graph),
                    filename="../figs/nvGraph/degree-distrib-{}.png".format(graph))


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
                    filename="../figs/nvGraph/shortest-path-distrib-{}.png".format(graph))


for graph in graphs:
  rel = [f for f in files if graph in f]
  for file in rel:
    if "degree" in file:
      plotDegrees(file, graph)
    elif "connectedComp" in file:
      plotConnectedComponents(file, graph)
    elif "clustering" in file:
      plotClusteringCoeff(file, graph)
    elif "SSSP." in file:
      plotSSSP(file, graph)
      # plotShortestPaths(file, graph)

# for graph in graphs:
#     for file in sssp_data:
#         plotSSSP(file, graph)