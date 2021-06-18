PORT=2020
RES_FILE=t

make
./server $PORT 1 1 random &
SPID=$!
sleep 0.05

./client localhost $PORT output.cgi &
./client localhost $PORT output.cgi &
./client localhost $PORT home.html &

kill $SPID
