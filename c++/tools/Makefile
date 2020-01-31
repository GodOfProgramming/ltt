#################
### Functions ###
#################

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

#################
### Variables ###
#################

CXX			:= g++
CXX_FLAGS 		:= -Wall -Wextra -std=c++17 -O3 -march=native -frename-registers -funroll-loops -fopenmp

SRC			:= src
BIN			:= bin

INCLUDE_DIRS		:= $(MAKE_INCLUDE_ONEOFFS)

################
### Targets  ###
################

all: make_dirs $(BIN)/hex-dump 

$(BIN)/hex-dump: $(SRC)/hex-dump.cpp
	$(CXX) $(CXX_FLAGS) -o $@ $(INCLUDE_DIRS) $<

make_dirs:
	-@mkdir -p $(BIN)

clean:
	-rm $(BIN)/*

FORCE:
