#!/bin/bash

if [[ -z "$TEST_ENV" ]]; then
  echo "NOT_SET"
else
  echo "$TEST_ENV"
fi

