#!/bin/bash

#relay="$1"
#input="$2"

# data="$(cat "$NN_FAKE_PACKET_BIN")"
# for ((;;)); do
# 	printf '$data' > /dev/udp/127.0.0.1/20000
# done

# printf "\x1invalidpacket" | iperf -u -c "127.0.0.1:20000" -b 1000M -B "127.0.0.1:51034" -I 
# printf "\x1invalidpacket" | nc -u 127.0.0.1 20000 -i 1

# udp-flood "$RELAY_ADDRESS" "$NN_FAKE_PACKET_BIN" "$(($(nproc)/2))" "2048"
udp-flood "$RELAY_ADDRESS" "$NN_FAKE_PACKET_BIN" "2" "2048"
