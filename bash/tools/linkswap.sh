#!/bin/bash

target="$1"
link="$2"

rm "$link"

ln -s "$target" "$link"

