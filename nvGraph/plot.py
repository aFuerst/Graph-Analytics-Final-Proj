from collections import defaultdict
import os
from csv import reader

import matplotlib as mpl
mpl.rcParams.update({"font.size": 14})
mpl.use("Agg")
import matplotlib.pyplot as plt
import argparse

graphs = ["gnutella", "wikivote", "hepth", "bitcoin", "enron", "hepph"]
sssp_data = ["graph_results/06_gnutella_SSSP.txt", "graph_results/02_wiki_SSSP.txt", "graph_results/04_hepth_SSSP.txt", "graph_results/03_bitcoin_SSSP.txt", "graph_results/01_email_SSSP.txt", "graph_results/05_ca_HepPh_SSSP.txt"]


def plot_distribution(data, xlabel='', ylabel='', title='', xlog=True, ylog=True, showLine=False, intAxis=False, filename=""):
  counts = {}
  i = 0
  for item in data:
      counts[i] = (i, item)
      i = i+1

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
        f.readline()
        r = reader(f)
        for short in r:
            SSSP.append(short)
    plot_distribution(SSSP, xlabel="Shortest Path Lengths ($hops$)",
                      ylabel="Number of Paths", title="Shortest Path Distributions for {}".format(graph),
                      filename="../figs/nvGraph/sssp-{}.png".format(graph))


for graph in graphs:
    for file in sssp_data:
        plotSSSP(file, graph)