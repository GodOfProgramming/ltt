#!/bin/bash

if [[ "$#" -eq "1" ]]; then
	echo "Generating full token"
	dist/tokens -token=new -expires=20s 		\
		-privatekey="$RELAY_PRIVATE_KEY" 			\
		-clientaddr="$NN_CLIENT_ADDR" 				\
		-clientpublickey="$RELAY_PUBLIC_KEY" 	\
		-relayaddr="$RELAY_ADDRESS" 					\
		-relaypublickey="$RELAY_PUBLIC_KEY" 	\
		-serveraddr="$NN_SERVER_ADDR" 				\
		-serverpublickey="$RELAY_PUBLIC_KEY" > "$NN_FAKE_PACKET_BIN"
else
	echo "Generating token without client"
	dist/tokens -token=new -expires=20s 					\
		-privatekey="$RELAY_ROUTER_PRIVATE_KEY" 		\
		-relayaddr="$RELAY_ADDRESS" 								\
		-relaypublickey="$RELAY_PUBLIC_KEY" 	\
		-serveraddr="$NN_SERVER_ADDR" 							\
		-serverpublickey="$RELAY_PUBLIC_KEY" > "$NN_FAKE_PACKET_BIN"
fi

