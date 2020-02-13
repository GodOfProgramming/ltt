#!/bin/bash

dist/tokens -token=new -expires=20s 		\
	-privatekey="$RELAY_PRIVATE_KEY" 			\
	-clientaddr="$NN_CLIENT_ADDR" 				\
	-clientpublickey="$RELAY_PUBLIC_KEY" 	\
	-relayaddr="$RELAY_ADDRESS" 					\
	-relaypublickey="$RELAY_PUBLIC_KEY" 	\
	-serveraddr="$NN_SERVER_ADDR" 				\
	-serverpublickey="$RELAY_PUBLIC_KEY" > "$NN_FAKE_PACKET_BIN"

