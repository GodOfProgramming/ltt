# cpp-repo-init.rb -e '../../bin/relay-bombarder' --shared 'pthread'

#################
### Functions ###
#################

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

#################
### Variables ###
#################

EXE                 := relay-bombarder

EXE_SPEC            := $(EXE).spec

CXX                 := g++

CXX_FLAGS           := -Wall -Wextra -std=c++17 -O3 -march=native -frename-registers -funroll-loops

SRC                 := src

INCLUDE             := include

SPEC                := spec

BIN                 := bin

OBJ                 := obj

SUB_DIRS            := $(shell find $(SRC)/* -type d -print)

OBJ_DIRS            := $(patsubst $(SRC)/%, $(OBJ)/%, $(SUB_DIRS))

MAIN                := main

MAIN_CXX            := $(MAIN).cxx

MAIN_OBJ            := $(MAIN).o

INCLUDE_DIRS        := -I$(INCLUDE) -I$(SRC)

INCLUDE_DIRS_SPEC   := $(INCLUDE_DIRS) -I$(CSPEC_INCLUDE)

STATIC_LIBS         :=

STATIC_LIBS_SPEC    := $(STATIC_LIBS)

SHARED_LIBS         := -lpthread

SHARED_LIBS_SPEC    := $(SHARED_LIBS) -lcspec

LIBRARY_DIRS        :=

LIBRARY_DIRS_SPEC   := $(LIBRARY_DIRS_SPEC)

INSTALL_DIR         :=

SRC_FILES           := $(call rwildcard, $(SRC), *.cpp)

SRC_OBJ_FILES       := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRC_FILES))

SRC_DEP_FILES       := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.d, $(SRC_FILES))

SPEC_FILES          := $(call rwildcard, $(SPEC), *.spec.cpp)

SPEC_OBJ_FILES      := $(patsubst $(SPEC)/%.spec.cpp, $(OBJ)/%.spec.o, $(SPEC_FILES))

SPEC_DEP_FILES      := $(patsubst $(SPEC)/%.spec.cpp, $(OBJ)/%.spec.d, $(SPEC_FILES))

PCH                 :=

DEPENDENCIES        := $(SRC_DEP_FILES) $(SPEC_DEP_FILES)

OBJECTS             := $(SRC_OBJ_FILES) $(SPEC_OBJ_FILES)

EXECUTABLES         := $(BIN)/$(EXE) $(BIN)/$(EXE_SPEC)

################
### Targets  ###
################

.PHONY: all
all: setup $(BIN)/$(EXE)

.PHONY: run-tests
run-tests: $(BIN)/$(EXE)
	@$<

.PHONY: run-benchmarks
run-benchmarks: $(BIN)/$(EXE)
	@$< --no-eval --bench

.PHONY: setup
setup: $(SRC) $(INCLUDE) $(BIN) $(OBJ) $(OBJ_DIRS)

.PHONY: clean
clean:
	-rm -f $(EXECUTABLES) $(OBJECTS) $(DEPENDENCIES)

.PHONY: force
force: clean all

.PHONY: check
check: $(BIN)/$(EXE)
	@$<
	@echo 'Passed!'

.PHONY: ci
ci: all check

-include $(DEPENDENCIES)

$(BIN)/$(EXE): $(SRC_OBJ_FILES) $(OBJ)/$(MAIN_OBJ)
	$(CXX) $(CXX_FLAGS) $(LIBRARY_DIRS) $^ $(STATIC_LIBS) -o $@ $(SHARED_LIBS)

$(OBJ)/$(MAIN_OBJ): $(SRC)/$(MAIN_CXX)
	$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS) $<

$(OBJ)/%.o: $(SRC)/%.cpp Makefile
	$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS) $<

$(SRC):
	@mkdir -p $@

$(INCLUDE):
	@mkdir -p $@

$(BIN):
	@mkdir -p $@

$(OBJ):
	@mkdir -p $@

$(OBJ_DIRS):
	@mkdir -p $@
