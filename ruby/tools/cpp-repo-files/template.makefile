# <%= derive_autogen_commands(options) %>

#################
### Functions ###
#################

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

#################
### Variables ###
#################
<% for var in all_vars %>
<%= var %>
<% end %>
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
<% unless options.pch.nil? %>
.PHONY: clear-pch
clear-pch:
	rm $(INCLUDE)/$(PCH).gch
<% end %>
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
$(INCLUDE)/$(PCH).gch: $(INCLUDE)/$(PCH) Makefile
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
