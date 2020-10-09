#!/bin/bash

a="$1"
b="$2"

if [ $a = 'on' ] || [ $b = 'on' ]; then
	echo 'on'
else
	echo 'off'
fi

