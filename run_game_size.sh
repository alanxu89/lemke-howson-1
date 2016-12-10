# compile
g++ -std=c++11 -O2 lemke_howson.cc -o main

# table_size=100
min_value=10
max_value=100
cap=10

# run
declare -a sizes=(10 20 30 40 50 60 70 80 90 100);

for table_size in ${sizes[@]};
do
    for i in `seq 1 30`;
    do
         echo "running for" "$cap"_"$table_size"
        ./gen $cap $table_size $table_size $min_value $max_value > buf
        ./main < buf >> table_sizes/heuristica_"$table_size".csv
        ./gen 100000 $table_size $table_size $min_value $max_value > buf
        ./main < buf >> table_sizes/normal_"$table_size".csv
    done
done

# for i in `seq 1 30`;
# do
#     echo "running for" out_"$cap"_"$i"
#     ./gen $cap $table_size $table_size $min_value $max_value > buf
#     ./main < buf >> lh_normal/heuristica_"$cap".csv
# done