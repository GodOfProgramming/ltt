CXX			:= g++
CXX_FLAGS 		:= -Wall -Wextra -std=c++17

SRC			:= src
INCLUDE			:= include
SPEC			:= spec

INCLUDE_DIRS		:= -I$(INCLUDE)
INCLUDE_DIRS_SPEC	:= -I$(CSPEC_INCLUDE)

STATIC_LIBS		:= 
STATIC_LIBS_SPEC	:= libcspec.a

LIBRARY_DIRS		:= -L$(HOME)/lib
LIBRARY_DIRS_SPEC	:= 

LIBRARIES		:= -lpthread
LIBRARIES_SPEC		:=

EXE			:= fiber.test
EXE_SPEC		:= fiber.spec

BIN			:= bin
OBJ			:= obj

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC_FILES	:= $(call rwildcard,$(SRC),*.cpp)
SRC_OBJ_FILES	:= $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRC_FILES))

SPEC_FILES	:= $(call rwildcard,$(SPEC),*.spec.cpp)
SPEC_OBJ_FILES	:= $(patsubst $(SPEC)/%.spec.cpp, $(OBJ)/%.spec.o, $(SPEC_FILES))

all: make_dirs $(BIN)/$(EXE) $(BIN)/$(EXE_SPEC)

$(BIN)/$(EXE): $(SRC_OBJ_FILES) $(SRC)/*.cxx
	$(CXX) $(CXX_FLAGS) $(INCLUDE_DIRS) $(LIBRARY_DIRS) $^ $(STATIC_LIBS) -o $@ $(LIBRARIES)

$(BIN)/$(EXE_SPEC): $(SPEC_OBJ_FILES) $(SRC_OBJ_FILES)
	$(CXX) $(CXX_FLAGS) $(LIBRARY_DIRS) $(LIBRARY_DIRS_SPEC) $^ $(STATIC_LIBS) $(STATIC_LIBS_SPEC) -o $@ $(LIBRARIES) $(LIBRARIES_SPEC)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $(INCLUDE_DIRS) $(LIBRARY_DIRS) $< -o $@ $(LIBRARIES)

$(OBJ)/%.spec.o: $(SPEC)/%.spec.cpp
	$(CXX) $(CXX_FLAGS) -c $(INCLUDE_DIRS) $(INCLUDE_DIRS_SPEC) $(LIBRARY_DIRS) $(LIBRARY_DIRS_SPEC) $< -o $@

make_dirs:
	-@mkdir -p $(BIN) $(OBJ)

clean:
	-rm $(BIN)/* $(OBJ)/*

FORCE:
