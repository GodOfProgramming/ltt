#!/bin/bash

desired_ubuntu_version="20.04"

ubuntu_version=$(lsb_release -r | grep -Po "([0-9]{2}\.[0-9]{2})")

if [[ ! "$ubuntu_version" == "$desired_ubuntu_version" ]]; then
  echo "This relay's ubuntu version is not valid."
  echo "This relay is on on $ubuntu_version."
  echo "Please update to $desired_ubuntu_version. "
fi

