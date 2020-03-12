require 'makefile_target'

class MakefileTargets
  DIRS = ['$(SRC)', '$(INCLUDE)', '$(SPEC)', '$(BIN)', '$(OBJ)', '$(OBJ_DIRS)']

  attr_reader :config

  def initialize(config)
    @config = config
  end

  def phony_targets
    return @phony_targets unless @phony_targets.nil?

    @phony_targets = [
      MakefileTarget.new(
        true, 
        'all', ['setup', '$(BIN)/$(EXE) $(BIN)/$(EXE_SPEC)']
      ),
      MakefileTarget.new(
        true, 
        'run-tests', ['$(BIN)/$(EXE)'],
        '@$<' 
      ),
      MakefileTarget.new(
        true, 
        'run-benchmarks', ['$(BIN)/$(EXE)'], 
        '@$< --no-eval --bench'
      ),
      MakefileTarget.new(
        true,
        'setup', DIRS
      ),
      MakefileTarget.new(
        true,
        'clean', nil, 
        '-rm -f $(EXECUTABLES) $(OBJECTS) $(DEPENDENCIES) ' + use_pch?("$(INCLUDE)/$(PCH).gch")
      ),
      MakefileTarget.new(
        true,
        'force', ['clean all']
      ),
      MakefileTarget.new(
        true,
        'check', ["$(BIN)/$(EXE)"],
        "@$<",
        "@echo 'Passed!'"
      ),
      MakefileTarget.new(
        true,
        'ci', ['all check']
      )
    ]

    unless config.precompiled_header.nil?
      @phony_targets.push(MakefileTarget.new(
        true,
        'clear-pch', nil,
        'rm $(INCLUDE)/$(PCH).gch' 
      ))
    end

    unless config.install_dir.nil?
      @phony_targets.push(MakefileTarget.new(
        true,
        'install', nil,
        "@echo 'Installing into $(INSTALL_DIR)",
        "@install $(BIN)/$(EXE) $(INSTALL_DIR)"
      ))
    end

    @phony_targets
  end

  def real_targets
    return @real_targets unless @real_targets.nil?
    @real_targets = [
      MakefileTarget.new(
        false,
        '$(BIN)/$(EXE)', ['$(SRC_OBJ_FILES) $(OBJ)/$(MAIN_OBJ)'],
        '$(CXX) $(CXX_FLAGS) $(LIBRARY_DIRS) $^ $(STATIC_LIBS) -o $@ $(SHARED_LIBS)' 
      ),
      MakefileTarget.new(
        false, 
        '$(BIN)/$(EXE_SPEC)', ['$(SPEC_OBJ_FILES) $(SRC_OBJ_FILES)'],
        '$(CXX) $(CXX_FLAGS) $(LIBRARY_DIRS_SPEC) $^ $(STATIC_LIBS_SPEC) -o $@ $(SHARED_LIBS_SPEC)'
      ),
      MakefileTarget.new(
        false, 
        '$(OBJ)/$(MAIN_OBJ)', ['$(SRC)/$(MAIN_CXX) ', use_pch?("$(INCLUDE)/$(PCH).gch")],
        '$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS) $<'
      ),
      MakefileTarget.new(
        false, 
        '$(OBJ)/%.o',  ['$(SRC)/%.cpp Makefile ', use_pch?("$(INCLUDE)/$(PCH).gch")],
        '$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS) $<'
      ),
      MakefileTarget.new(
        false,
        '$(OBJ)/%.spec.o', ['$(SPEC)/%.spec.cpp Makefile ', use_pch?("$(INCLUDE)/$(PCH).gch")],
        '$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS_SPEC) $<'
      )
    ]

    unless config.precompiled_header.nil?
      @real_targets.push(MakefileTarget.new(
        false,
        '$(INCLUDE)/$(PCH).gch', ['$(INCLUDE)/$(PCH) Makefile'],
        '$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP -x c++-header $(INCLUDE_DIRS) $<'
      ))
    end

    @real_targets += DIRS.map{ |dir| MakefileTarget.new(
      false,
      dir, nil,
      '@mkdir -p $@',
    )}

    @real_targets
  end

  private

  def use_pch?(val)
    "#{ unless config.precompiled_header.nil?; val; end }"
  end
end
