import networkx as nx
import scipy as sp
import numpy as np
from scipy import io


def patch_email():
    print("Patching and Converting email-Enron.txt")
    mapped = {}
    i = 0
    G = nx.Graph()
    with open("../graphs/email-Enron.txt") as f:
        for line in f.readlines():
            if line[0] == '#':
                continue
            src, dest = line.split()
            if src in mapped:
                src = mapped[src]
            else:      
                mapped[src] = i
                src = mapped[src]
                i += 1

            if dest in mapped:
                dest = mapped[dest]
            else:      
                mapped[dest] = i
                dest = mapped[dest]
                i += 1

            G.add_edge(src, dest)
        
    G2 = nx.to_scipy_sparse_matrix(G, format='csc')
    np.savetxt("graph_data/01_email_weights.txt", G2.data)
    np.savetxt("graph_data/01_email_indices.txt", G2.indices)
    np.savetxt("graph_data/01_email_offsets.txt", G2.indptr)

def patch_wiki_vote():
    print("Patching and Converting wiki-Vote.txt")
    mapped = {}
    i = 0
    G = nx.DiGraph()
    with open("../graphs/wiki-Vote.txt") as f:
        for line in f.readlines():
            if line[0] == '#':
                continue
            src, dest = line.split()
            if src in mapped:
                src = mapped[src]
            else:      
                mapped[src] = i
                src = mapped[src]
                i += 1

            if dest in mapped:
                dest = mapped[dest]
            else:      
                mapped[dest] = i
                dest = mapped[dest]
                i += 1

            G.add_edge(src, dest)

    G2 = nx.to_scipy_sparse_matrix(G, format='csc')
    np.savetxt("graph_data/02_wiki_weights.txt", G2.data)
    np.savetxt("graph_data/02_wiki_indices.txt", G2.indices)
    np.savetxt("graph_data/02_wiki_offsets.txt", G2.indptr)

def patch_bitcoin():
    print("Patching and Converting soc-sign-bitcoinotc.csv")
    mapped = {}
    i = 0
    G = nx.DiGraph()
    with open("../graphs/soc-sign-bitcoinotc.csv") as f:
        for line in f.readlines():
            src, dest, a, b = line.split(",")
            if src in mapped:
                src = mapped[src]
            else:      
                mapped[src] = i
                src = mapped[src]
                i += 1

            if dest in mapped:
                dest = mapped[dest]
            else:      
                mapped[dest] = i
                dest = mapped[dest]
                i += 1

            G.add_edge(src, dest)

    G2 = nx.to_scipy_sparse_matrix(G, format='csc')
    np.savetxt("graph_data/03_bitcoin_weights.txt", G2.data)
    np.savetxt("graph_data/03_bitcoin_indices.txt", G2.indices)
    np.savetxt("graph_data/03_bitcoin_offsets.txt", G2.indptr)

def patch_hepth():
    print("Patching and Converting cit-HepTh.txt")
    mapped = {}
    i = 0
    G = nx.DiGraph()
    with open("../graphs/cit-HepTh.txt") as f:
        for line in f.readlines():
            if line[0] == '#':
                continue
            src, dest = line.split()
            if src in mapped:
                src = mapped[src]
            else:      
                mapped[src] = i
                src = mapped[src]
                i += 1

            if dest in mapped:
                dest = mapped[dest]
            else:      
                mapped[dest] = i
                dest = mapped[dest]
                i += 1

            G.add_edge(src, dest)
    
    G2 = nx.to_scipy_sparse_matrix(G, format='csc')
    np.savetxt("graph_data/04_hepth_weights.txt", G2.data)
    np.savetxt("graph_data/04_hepth_indices.txt", G2.indices)
    np.savetxt("graph_data/04_hepth_offsets.txt", G2.indptr)

def patch_ca_HepPh():
    print("Patching and Converting ca-HepPh.txt")
    mapped = {}
    i = 0
    G = nx.Graph()
    with open("../graphs/ca-HepPh.txt") as f:
        for line in f.readlines():
            if line[0] == '#':
                continue
            src, dest = line.split()
            if src in mapped:
                src = mapped[src]
            else:      
                mapped[src] = i
                src = mapped[src]
                i += 1

            if dest in mapped:
                dest = mapped[dest]
            else:      
                mapped[dest] = i
                dest = mapped[dest]
                i += 1

            G.add_edge(src, dest)
    
    G2 = nx.to_scipy_sparse_matrix(G, format='csc')
    np.savetxt("graph_data/05_ca_HepPh_weights.txt", G2.data)
    np.savetxt("graph_data/05_ca_HepPh_indices.txt", G2.indices)
    np.savetxt("graph_data/05_ca_HepPh_offsets.txt", G2.indptr)

def patch_gnutella():
    print("Patching and Converting p2p-Gnutella31.txt")
    mapped = {}
    i = 0
    G = nx.DiGraph()
    with open("../graphs/p2p-Gnutella31.txt") as f:
        for line in f.readlines():
            if line[0] == '#':
                continue
            src, dest = line.split()
            if src in mapped:
                src = mapped[src]
            else:      
                mapped[src] = i
                src = mapped[src]
                i += 1

            if dest in mapped:
                dest = mapped[dest]
            else:      
                mapped[dest] = i
                dest = mapped[dest]
                i += 1

            G.add_edge(src, dest)
    
    G2 = nx.to_scipy_sparse_matrix(G, format='csc')
    np.savetxt("graph_data/06_gnutella_weights.txt", G2.data)
    np.savetxt("graph_data/06_gnutella_indices.txt", G2.indices)
    np.savetxt("graph_data/06_gnutella_offsets.txt", G2.indptr)

patch_email()
patch_wiki_vote()
patch_bitcoin()
patch_hepth()
patch_ca_HepPh()
patch_gnutella()