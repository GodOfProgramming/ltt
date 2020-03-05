#!/bin/bash

echo "Generating route token without client"
dist/tokens -token='new' -expires='20s' 									\
	-privatekey="$RELAY_ROUTER_PRIVATE_KEY" 								\
	-relayaddr="$RELAY_ADDRESS" 														\
	-relaypublickey="$RELAY_PUBLIC_KEY" 										\
	-serveraddr="$NN_SERVER_ADDR" 													\
	-serverpublickey="$RELAY_PUBLIC_KEY" > "$NEW_TOKEN_BIN"

echo "Generating continue token without client"
dist/tokens -token='continue' -expires='10s' 							\
	-privatekey="$RELAY_ROUTER_PRIVATE_KEY" 								\
	-relayaddr="$RELAY_ADDRESS" 														\
	-relaypublickey="$RELAY_PUBLIC_KEY" 										\
	-serveraddr="$NN_SERVER_ADDR" 													\
	-serverpublickey="$RELAY_PUBLIC_KEY" > "$CONT_TOKEN_BIN"

