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

mapped = {}
i = 0
with open("../graphs/social/soc-sign-bitcoinotc-patch.csv", "w") as patch:
  with open("../graphs/social/soc-sign-bitcoinotc.csv") as f:
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
