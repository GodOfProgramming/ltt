#!/bin/bash

for ((irq=120; irq<=137; irq++)); do
	cat /proc/irq/$irq/affinity_hint
done
