# <%= derive_autogen_commands(options) %>

#################
### Functions ###
#################

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

#################
### Variables ###
#################

EXE					:= <%= options.exe %>
EXE_SPEC			:= $(EXE).spec

CXX					:= <%= options.cxx %>
CXX_FLAGS 			:= <%= (options.cxx_flags + DEFAULT_CXX_FLAGS).map{ |f| "-#{f}" }.join(' ') %>

SRC					:= src
INCLUDE				:= include
SPEC				:= spec
BIN					:= bin
OBJ					:= obj

SUB_DIRS			:= $(shell find $(SRC)/* -type d -print)
OBJ_DIRS			:= $(patsubst $(SRC)/%, $(OBJ)/%, $(SUB_DIRS))

MAIN				:= main
MAIN_CXX			:= $(MAIN).cxx
MAIN_OBJ			:= $(MAIN).out

INCLUDE_DIRS		:= -I$(INCLUDE) -I$(SRC) <%= options.include.map{ |i| "-I#{i}" }.join(' ') unless options.include.empty? %> <%= use_ltt?(options) %>
INCLUDE_DIRS_SPEC	:= $(INCLUDE_DIRS) -I$(CSPEC_INCLUDE) <%= options.include_spec.map{ |i| "-I#{i}" }.join(' ') unless options.include_spec.empty? %>

STATIC_LIBS			:= <%= options.static.join(' ') unless options.static.empty? %>
STATIC_LIBS_SPEC	:= $(STATIC_LIBS) <%= options.static_spec.join(' ') unless options.static_spec.empty? %>

SHARED_LIBS			:= <%= options.shared.map{ |s| "-l#{s}" }.join(' ') unless options.shared.empty? %>
SHARED_LIBS_SPEC	:= $(SHARED_LIBS) -lcspec <%= options.shared_spec.map{ |s| "-l#{s}" }.join(' ') unless options.shared_spec.empty? %>

LIBRARY_DIRS		:= <%= options.library_dirs.map{ |l| "-L#{l}" }.join(' ') unless options.library_dirs.empty? %>
LIBRARY_DIRS_SPEC	:= $(LIBRARY_DIRS) <%= options.library_dirs_spec.map{ |l| "-L#{l}" }.join(' ') unless options.library_dirs_spec.empty? %>
<% if options.install %>
INSTALL_DIR			:= <%= options.install %>
<% end %>
SRC_FILES			:= $(call rwildcard,$(SRC),*.cpp)

SRC_OBJ_FILES		:= $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRC_FILES))
SRC_DEP_FILES		:= $(patsubst $(SRC)/%.cpp, $(OBJ)/%.d, $(SRC_FILES))

SPEC_FILES			:= $(call rwildcard,$(SPEC),*.spec.cpp)
SPEC_OBJ_FILES		:= $(patsubst $(SPEC)/%.spec.cpp, $(OBJ)/%.spec.o, $(SPEC_FILES))
SPEC_DEP_FILES		:= $(patsubst $(SPEC)/%.spec.cpp, $(OBJ)/%.spec.d, $(SPEC_FILES))
<% if options.pch %>
PCH					:= <%= options.pch  %>
<% end %>
DEPENDENCIES		:= $(SRC_DEP_FILES) $(SPEC_DEP_FILES)

OBJECTS				:= $(SRC_OBJ_FILES) $(SPEC_OBJ_FILES)
EXECUTABLES			:= $(EXE) $(EXE_SPEC)

################
### Targets  ###
################

.PHONY: all
all: setup $(BIN)/$(EXE) $(BIN)/$(EXE_SPEC)

.PHONY: run-tests
run-tests: $(BIN)/$(EXE_SPEC)
	@$<

.PHONY: run-benchmarks
run-benchmarks: $(BIN)/$(EXE_SPEC)
	@$< --no-eval --bench

.PHONY: setup
setup: <%= setup_dirs %>

.PHONY: clean
clean:
	-rm -f $(EXECUTABLES) $(OBJECTS) $(DEPENDENCIES) <%= if options.pch; "$(INCLUDE)/$(PCH).gch"; end %>

.PHONY: force
force: clean all
<% if options.install %>
.PHONY: install
install:
	@echo "Installing into $(INSTALL_DIR)"
	@install $(BIN)/$(EXE) $(INSTALL_DIR)
<% end %>
.PHONY: check
check: $(BIN)/$(EXE_SPEC)
	@$<
	@echo "Passed!"

.PHONY: ci
ci: all check

-include $(DEPENDENCIES)

$(BIN)/$(EXE): $(SRC_OBJ_FILES) $(OBJ)/$(MAIN_OBJ)
	$(CXX) $(CXX_FLAGS) $(LIBRARY_DIRS) $^ $(STATIC_LIBS) -o $@ $(SHARED_LIBS)

$(BIN)/$(EXE_SPEC): $(SPEC_OBJ_FILES) $(SRC_OBJ_FILES)
	$(CXX) $(CXX_FLAGS) $(LIBRARY_DIRS_SPEC) $^ $(STATIC_LIBS_SPEC) -o $@ $(SHARED_LIBS_SPEC)
<% unless options.pch.nil? %>
$(INCLUDE)/$(PCH).gch: $(INCLUDE)/$(PCH)
	$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP -x c++-header $(INCLUDE_DIRS) $< 
<% end %>
$(OBJ)/$(MAIN_OBJ): $(SRC)/$(MAIN_CXX) <%= if options.pch; "$(INCLUDE)/$(PCH).gch"; end %>
	$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS) $< 

$(OBJ)/%.o: $(SRC)/%.cpp Makefile <%= if options.pch; "$(INCLUDE)/$(PCH).gch"; end %>
	$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS) $< 

$(OBJ)/%.spec.o: $(SPEC)/%.spec.cpp Makefile <%= if options.pch; "$(INCLUDE)/$(PCH).gch"; end %>
	$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS_SPEC) $< 
<% for dir in DIRS %>
<%= dir %>:
	@mkdir -p $@
<% end %>
