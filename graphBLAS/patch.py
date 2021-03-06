from scipy import io
import networkx as nx

# unique_src = set()
# unique_dest = set()
# with open("../graphs/social/soc-sign-bitcoinotc.csv") as f:
#   for line in f.readlines():
#     src, dest, _, _ = line.split(",")
#     unique_src.add(src)
#     unique_dest.add(dest)

# print(len(unique_src))
# print(len(unique_dest))

# print(len(unique_src.union(unique_dest)))

"""
Ensure all the values in dataset are legit
"""
def patch_bitcoin():
  mapped = {}
  i = 0
  with open("../graphs/soc-sign-bitcoinotc-patch.csv", "w") as patch:
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

        patch.write("{},{},{},{}".format(src,dest,a,b))

def patch_enron():
  print("patching and converting email-Enron.txt")
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
  sp = nx.to_scipy_sparse_matrix(G)
  io.mmwrite("../graphs/email-Enron-patched.mtx", sp)

def patch_hepth():
  print("patching and converting cit-HepTh.txt")
  mapped = {}
  i = 0
  G = nx.Graph()
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
  sp = nx.to_scipy_sparse_matrix(G)
  io.mmwrite("../graphs/cit-HepTh-patched.mtx", sp)

def patch_gnutella():
  print("patching and converting p2p-Gnutella31.txt")
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
  sp = nx.to_scipy_sparse_matrix(G)
  io.mmwrite("../graphs/p2p-Gnutella31-patched.mtx", sp)

def patch_hepph():
  print("patching and converting ca-HepPh.txt")
  mapped = {}
  i = 0
  G = nx.DiGraph()
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
  sp = nx.to_scipy_sparse_matrix(G)
  io.mmwrite("../graphs/ca-HepPh-patched.mtx", sp)

def patch_wiki_vote():
  print("patching and converting wiki-Vote.txt")
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
  sp = nx.to_scipy_sparse_matrix(G)
  io.mmwrite("../graphs/wiki-Vote-patched.mtx", sp)

patch_bitcoin()
patch_enron()
patch_hepth()
patch_gnutella()
patch_hepph()
patch_wiki_vote()
