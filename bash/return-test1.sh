#!/bin/bash

me=$(realpath "$0")
cur_dir=$(dirname "$me")

echo "$me"
echo "$cur_dir"

source "$cur_dir/tests/return-test2.sh" && echo 'returned true'

