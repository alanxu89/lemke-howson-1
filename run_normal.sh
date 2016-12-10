# compile
g++ -std=c++11 -O2 lemke_howson.cc -o main

table_size=100
min_value=10
max_value=100

# run
for cap in `seq 1 35`;
do
    for i in `seq 1 30`;
    do
        echo "running for" out_"$cap"_"$i"
        ./gen $cap $table_size $table_size $min_value $max_value > buf
        ./main < buf >> lh_normal/heuristica_"$cap".csv
    done
done  