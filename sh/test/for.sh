#!/bin/sh

out=0
while [ $out -eq 0 ]; do
	read line
	echo "you said $line"

	if [ "$line" = 'done' ]; then
		out=1
	fi
done

