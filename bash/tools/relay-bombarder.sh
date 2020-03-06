#!/bin/bash

if [[ "$1" == "route" ]]; then
	udp-flood "$RELAY_ADDRESS" "2" $[2**18] "$NEW_TOKEN_BIN"
elif [[ "$1" == "cont" ]]; then
	relay-bombarder "$RELAY_ADDRESS" $[2**18] "2" 
else
	echo "use a valid command dumbass, you used '$1'"
fi
