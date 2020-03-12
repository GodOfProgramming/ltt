require 'makefile_var'

class MakefileVars
  attr_reader :config

  def initialize(config)
    @config = config
  end

  def all
    @all ||= [
      MakefileVar.new('EXE', config.exe),
      MakefileVar.new('EXE_SPEC', '$(EXE).spec'),
      MakefileVar.new('CXX', config.cxx),
      MakefileVar.new('CXX_FLAGS', config.cxx_flags.join(' ')),
      MakefileVar.new('SRC', 'src'),
      MakefileVar.new('INCLUDE', 'include'),
      MakefileVar.new('SPEC', 'spec'),
      MakefileVar.new('BIN', 'bin'),
      MakefileVar.new('OBJ', 'obj'),
      MakefileVar.new('SUB_DIRS', '$(shell find $(SRC)/* -type d -print)'),
      MakefileVar.new('OBJ_DIRS', '$(patsubst $(SRC)/%, $(OBJ)/%, $(SUB_DIRS))'),
      MakefileVar.new('MAIN', 'main'),
      MakefileVar.new('MAIN_CXX', '$(MAIN).cxx'),
      MakefileVar.new('MAIN_OBJ', '$(MAIN).o'),
      MakefileVar.new('INCLUDE_DIRS', expand_flags(config.include_dirs, '-I'), use_ltt?),
      MakefileVar.new('INCLUDE_DIRS_SPEC', expand_flags(config.include_dirs_spec, '-I')),
      MakefileVar.new('STATIC_LIBS', expand_flags(config.static_libs, nil)),
      MakefileVar.new('STATIC_LIBS_SPEC', expand_flags(config.static_libs_spec, nil)),
      MakefileVar.new('SHARED_LIBS', expand_flags(config.shared_libs, '-l')),
      MakefileVar.new('SHARED_LIBS_SPEC', expand_flags(config.shared_libs_spec, '-l')),
      MakefileVar.new('LIBRARY_DIRS', expand_flags(config.library_dirs, '-L')),
      MakefileVar.new('LIBRARY_DIRS_SPEC', expand_flags(config.library_dirs_spec, '-L')),
      MakefileVar.new('INSTALL_DIR', config.install_dir),
      MakefileVar.new('SRC_FILES', '$(call rwildcard, $(SRC), *.cpp)'),
      MakefileVar.new('SRC_OBJ_FILES', '$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRC_FILES))'),
      MakefileVar.new('SRC_DEP_FILES', '$(patsubst $(SRC)/%.cpp, $(OBJ)/%.d, $(SRC_FILES))'),
      MakefileVar.new('SPEC_FILES', '$(call rwildcard, $(SPEC), *.spec.cpp)'),
      MakefileVar.new('SPEC_OBJ_FILES', '$(patsubst $(SPEC)/%.spec.cpp, $(OBJ)/%.spec.o, $(SPEC_FILES))'),
      MakefileVar.new('SPEC_DEP_FILES', '$(patsubst $(SPEC)/%.spec.cpp, $(OBJ)/%.spec.d, $(SPEC_FILES))'),
      MakefileVar.new('PCH', config.precompiled_header),
      MakefileVar.new('DEPENDENCIES', '$(SRC_DEP_FILES) $(SPEC_DEP_FILES) ', use_pch?),
      MakefileVar.new('OBJECTS', '$(SRC_OBJ_FILES) $(SPEC_OBJ_FILES)'),
      MakefileVar.new('EXECUTABLES', '$(BIN)/$(EXE) $(BIN)/$(EXE_SPEC)')
    ]
  end

  private

  def expand_flags(opts, flag)
    opts.map{ |i| "#{flag}#{i}" }.join(' ')
  end

  def use_ltt?
    "#{ if config.use_ltt; '$(MAKE_INCLUDE_HELPER)'; end }"
  end

  def use_pch?
    "#{ unless config.precompiled_header.nil?; "$(OBJ)/$(PCH).d"; end }"
  end
end
