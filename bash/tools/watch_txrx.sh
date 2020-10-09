#!/bin/bash

readarray -t interfaces < <(ip link | awk -F: '$0 !~ "lo|vir|wl|^[^0-9]"{print $2;getline}')

num_interfaces=${#interfaces[@]}

echo "number of interfaces: $num_interfaces"

echo 'select one of the below:'
for ((i=0; i<${#interfaces[@]}; i++)) {
	echo "${interfaces[$i]} [$i]"
}

selected_interface=-1

while [ $selected_interface -lt 0 ]; do
	read -n 1 -p 'choice: ' choice
	echo ''
	if [[ $choice =~ ^[0-9]+$ ]] && [[ $choice -ge 0 ]] && [[ $choice -lt $num_interfaces ]]; then
		selected_interface="$choice"
	else
		echo "invalid choice '$choice', select again"
	fi
done

interface="${interfaces[$selected_interface]//[$'\t\r\n ']}"

echo "selected interface: $interface"

get_rx() {
        rx=$(cat /sys/class/net/$interface/statistics/rx_packets)
}

get_tx() {
        tx=$(cat /sys/class/net/$interface/statistics/tx_packets)
}

while true; do
        rx=0
        tx=0
        get_rx
        get_tx
        before_rx=$rx
        before_tx=$tx
        sleep 1
        get_rx
        get_tx
        after_rx=$rx
        after_tx=$tx

        rx=$((after_rx - before_rx))
        tx=$((after_tx - before_tx))
        echo "rx = $rx"
        echo "tx = $tx"
        echo "total = $((rx + tx))"
done
