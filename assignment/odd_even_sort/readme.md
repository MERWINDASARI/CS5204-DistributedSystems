issue: corner cases 

my idea is to divide each pair of elements to available process so that in
one odd phase no need of message passing and in even phase each process send its data to rank+1 process and receive from rank-1
expect for last and first respectivily
