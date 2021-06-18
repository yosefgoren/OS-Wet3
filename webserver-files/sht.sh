PORT=5000
INTERVAL=0.01

./kills.sh >& /dev/null
make > /dev/null

./server $PORT 2 3 dt &
SPID=$!
sleep 0.2

./client localhost $PORT output.cgi >& tmp/tmp1 &
CPID1=$!
sleep 0.01
./client localhost $PORT output.cgi >& tmp/tmp2 &
CPID2=$!
sleep 0.01
./client localhost $PORT output.cgi >& tmp/tmp3 &
CPID3=$!
sleep 0.01
./client localhost $PORT errr >& tmp/tmp4 &
CPID4=$!
sleep 0.01
./client localhost $PORT output.cgi >& tmp/tmp5 &
CPID5=$!
sleep 0.01
./client localhost $PORT errr >& tmp/tmp6 &
CPID6=$!
sleep 0.01

#declare -A CPIS=()
# for INDEX in 1 2 .. 5
# do
#     ./client localhost $PORT output.cgi >& tmp/tmp$INDEX &
#     CPIS[$INDEX]=$!
#     sleep $INTERVAL
# done

# for INDEX in 6 7 .. 11
# do
#     ./client localhost $PORT home.html >& tmp/tmp$INDEX &
#     CPIS[$INDEX]=$!
#     sleep $INTERVAL
# done

# for INDEX in 1 2 .. 11
# do
#     wait CPIS[$INDEX]
# done

wait $CPID1
wait $CPID2
wait $CPID3
wait $CPID4
wait $CPID5
wait $CPID6

kill $SPID
