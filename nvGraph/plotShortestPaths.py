from collections import defaultdict
import os
from csv import reader

import matplotlib as mpl
mpl.rcParams.update({'font.size': 14})
mpl.use('Agg')
import matplotlib.pyplot as plt
import argparse

graphs = ["gnutella", "wikivote", "hepth", "bitcoin", "enron", "hepph"]
times = ["graph_results/06_gnutella_SSSP_time.txt", "graph_results/02_wiki_SSSP_time.txt", "graph_results/04_hepth_SSSP_time.txt", "graph_results/03_bitcoin_SSSP_time.txt", "graph_results/01_email_SSSP_time.txt", "graph_results/05_ca_HepPh_SSSP_time.txt"]

#gb_data = "./graphBLAS/savedata"
#gb_files = os.listdir(gb_data)
#gb_files = [os.path.join(gb_data, f) for f in gb_files]

def plotShortestPathTimes():
    gb_times = defaultdict(list)
    for file in times:
        with open(file) as f:
            f.readline()
            r = reader(f)
            tot_time = r.__next__()
            for time in r:
                gb_times[times].append(float(time))

    fig, ax = plt.subplots()
    plt.tight_layout()
    fig.set_size_inches(5,3)
    ax.boxplot(gb_times.values(), labels=list(gb_times.keys()), vert=False)

    ax.set_xlabel("Iteration Time (mico sec)")
    # ax.set_yscale ('log')
    # ax.set_xlabel("")
    # ax.legend(bbox_to_anchor=(1,.6), loc="right", columnspacing=1)
    plt.savefig("../figs/shortestpath_gpu.png", bbox_inches="tight")
    plt.close(fig)


plotShortestPathTimes()