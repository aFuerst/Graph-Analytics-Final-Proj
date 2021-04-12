base="graphs"
mkdir -p $base
social="$base/social"
mkdir -p $social

# http://snap.stanford.edu/data/soc-LiveJournal1.html
# wget  http://snap.stanford.edu/data/soc-LiveJournal1.txt.gz -O "$social/soc-LiveJournal1.txt.gz" 

# http://snap.stanford.edu/data/soc-sign-bitcoin-otc.html
wget https://snap.stanford.edu/data/soc-sign-bitcoinotc.csv.gz -O "$social/soc-sign-bitcoinotc.csv.gz" 
gzip -d "$social/soc-sign-bitcoinotc.csv.gz"