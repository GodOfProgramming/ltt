#!/bin/bash

if systemctl is-active --quiet redis; then
	echo 'redis is active'
else
	echo 'redis is inactive'
fi

