#!/bin/bash

while getopts 'h' flag; do
	case "$flag" in
		h)
			echo 'Reroute [ALIAS] as [IP]'
			echo 'reroute.sh [-h] <ALIAS> <IP>'
			exit 0 
			;;
		*)
			echo 'invalid argument given'
			exit 0
			;;
	esac
done

alias="$1"
ip="$2"

sudo iptables -t nat -A OUTPUT -d "$alias" -j DNAT --to-destination "$ip"

