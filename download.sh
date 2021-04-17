base="graphs"
mkdir -p $base

# http://snap.stanford.edu/data/soc-LiveJournal1.html
# wget  http://snap.stanford.edu/data/soc-LiveJournal1.txt.gz -O "$social/soc-LiveJournal1.txt.gz" 

graphs=("soc-sign-bitcoinotc.csv.gz" "cit-HepTh.txt.gz" "p2p-Gnutella31.txt.gz" "wiki-Vote.txt.gz" "email-Enron.txt.gz" "ca-HepPh.txt.gz")

# http://snap.stanford.edu/data/soc-sign-bitcoin-otc.html
# http://snap.stanford.edu/data/cit-HepTh.html
# http://snap.stanford.edu/data/p2p-Gnutella31.html
# http://snap.stanford.edu/data/wiki-Vote.html
# http://snap.stanford.edu/data/email-Enron.html
# http://snap.stanford.edu/data/ca-HepPh.html

for g in "${graphs[@]}"
do
  wget "https://snap.stanford.edu/data/$g" -O "$base/$g" 
  gzip -f -d "$base/$g"
done