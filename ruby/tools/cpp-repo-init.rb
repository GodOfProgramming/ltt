#!/bin/ruby
require 'optparse'
require 'ostruct'
require 'sys_helpers'
require 'fileutils'
require 'erb'
require 'makefile_var'
require 'makefile_target'

######################
### Option Parsing ###
######################

options = OpenStruct.new
options.exe = SysHelpers.this_dir
options.cxx = 'g++'
options.cxx_flags =  []
options.include = []
options.include_spec = []
options.static = []
options.static_spec = []
options.shared = []
options.shared_spec = []
options.library_dirs = []
options.library_dirs_spec = []
options.install = nil
options.use_ltt = nil
options.remake = nil
options.pch = nil
options.install_ycm = false
options.install_clang_format = false

DEFAULT_CXX_FLAGS = ["Wall", "Wextra", "std=c++17", "O3", "march=native", "frename-registers", "funroll-loops"]

OptionParser.new() do |opts|
  opts.on("-e", "--exe [NAME]", "The name of the executable") do |v|
    options.exe = v
  end

  opts.on("-c", "--cxx [CXX]", "The compiler to use, can be gcc or g++") do |v|
    options.cxx = v
  end

  opts.on("-f", "--flag [FLAG]", "A cxx flag; Flags must be surrounded in quotes") do |v|
    options.cxx_flags.push v
  end

  opts.on("-r", "--remake", "Rerun this script using the settings from the last time") do |v|
    options.remake = v
  end

  opts.on("--include [INCLUDE]", "An include dir to use") do |v|
    options.include.push v
  end

  opts.on("--include-spec [INCLUDE]", "An include dir to use for specs") do |v|
    options.include_spec.push v
  end

  opts.on("--static [LIB]", "A static library to use") do |v|
    options.static.push v
  end

  opts.on("--static-spec [LIB]", "A static library to use when building specs") do |v|
    options.static_spec.push v
  end

  opts.on("--shared [LIB]", "A shared library to use") do |v|
    options.shared.push v
  end

  opts.on("--shared-spec [LIB]", "A shared library to use when building specs") do |v|
    options.shared_spec.push v
  end

  opts.on("--library [LIB]", "A library directory") do |v|
    options.library_dirs.push v
  end

  opts.on("--library-spec [LIB]", "A library directory when building specs") do |v|
    options.library_dirs_spec.push v
  end

  opts.on("--install [DIR]", "Create a target to install to DIR") do |v|
    options.install = v
  end

  opts.on("--use-ltt", "Enable the ltt repo libs") do |v|
    options.use_ltt = v
  end

  opts.on("--pch [FILE]", "Enable precompiled headers") do |v|
    if v[/.+\.h/].nil?
      puts "--pch was used with an invalid file name, please specify a *.h file"
      exit 1
    else
      options.pch = v
    end
  end

  opts.on("--ycm", "Copy the YouCompleteMe extra conf file") do |v|
    options.install_ycm = v
  end

  opts.on("--clang-format", "Copy the clang-format file") do |v|
    options.install_clang_format = v
  end
end.parse!

if options.install_ycm
  FileUtils.cp("#{__dir__}/cpp-repo-files/.ycm_extra_conf.py", Dir.pwd)
  exit 0
end

if options.install_clang_format
  FileUtils.cp("#{__dir__}/cpp-repo-files/.clang-format", Dir.pwd)
  exit 0
end

if options.remake
  if File.exists? 'Makefile'
    line1 = File.open('Makefile', &:readline)
    cmdstr = line1.split[1..-1].join(' ')
    system(cmdstr)
  else
    puts "please generate a Makefile first"
  end
  exit 0
end

###########################
### Makefile Generation ###
###########################

if options.exe.empty?
  puts "-e, --exe is required"
  exit 1
end

if options.cxx.empty?
  puts "-c, --cxx is required"
  exit 1
end

def derive_autogen_commands(opts)
  str = File.basename(__FILE__)
  str += " -e '#{opts.exe}'" if opts.exe != SysHelpers.this_dir
  str += " -c '#{opts.cxx}'" if opts.cxx != 'g++'
  str += " #{opts.cxx_flags.map{ |f| "-f '#{f}'" }.join(' ')}" unless opts.cxx_flags.empty?
  str += " #{opts.include.map{ |i| "--include '#{i}'" }.join(' ')}" unless opts.include.empty?
  str += " #{opts.include_spec.map { |i| "--include-spec '#{i}'" }.join(' ')}" unless opts.include_spec.empty?
  str += " #{opts.static.map{ |s| "--static '#{s}'" }.join(' ')}" unless opts.static.empty?
  str += " #{opts.static_spec.map{ |s| "--static-spec '#{s}'" }.join(' ')}" unless opts.static_spec.empty?
  str += " #{opts.shared.map{ |s| "--shared '#{s}'" }.join(' ')}" unless opts.shared.empty?
  str += " #{opts.shared_spec.map{ |s| "--shared-spec '#{s}'" }.join(' ')}" unless opts.shared_spec.empty?
  str += " --install" if opts.install
  str += " --use-ltt" if opts.use_ltt
  str += " --pch '#{opts.pch}'" if opts.pch
  str
end

def expand_flags(opts, flag)
  opts.map{ |i| "#{flag}#{i}" }.join(' ')
end

def use_ltt?(opts)
  "#{if opts.use_ltt; '$(MAKE_INCLUDE_HELPER)'; end }"
end

DIRS = ['$(SRC)', '$(INCLUDE)', '$(SPEC)', '$(BIN)', '$(OBJ)', '$(OBJ_DIRS)']

all_vars = [
  MakefileVar.new('EXE', options.exe),
  MakefileVar.new('EXE_SPEC', '$(EXE).spec'),
  MakefileVar.new('CXX', options.cxx),
  MakefileVar.new('CXX_FLAGS', expand_flags([*options.cxx_flags, *DEFAULT_CXX_FLAGS], '-')),
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
  MakefileVar.new('INCLUDE_DIRS', '-I$(INCLUDE) -I$(SRC) ', expand_flags(options.include, '-I'), use_ltt?(options)),
  MakefileVar.new('INCLUDE_DIRS_SPEC', '$(INCLUDE_DIRS) -I$(CSPEC_INCLUDE) ', expand_flags(options.include_spec, '-I')),
  MakefileVar.new('STATIC_LIBS', expand_flags(options.static, nil)),
  MakefileVar.new('STATIC_LIBS_SPEC', '$(STATIC_LIBS) ', expand_flags(options.static_spec, nil)),
  MakefileVar.new('SHARED_LIBS', expand_flags(options.shared, '-l')),
  MakefileVar.new('SHARED_LIBS_SPEC', '$(SHARED_LIBS) -lcspec ', expand_flags(options.shared_spec, '-l')),
  MakefileVar.new('LIBRARY_DIRS', expand_flags(options.library_dirs, '-L')),
  MakefileVar.new('LIBRARY_DIRS_SPEC', '$(LIBRARY_DIRS_SPEC) ', expand_flags(options.library_dirs_spec, '-L')),
  MakefileVar.new('INSTALL_DIR', options.install),
  MakefileVar.new('SRC_FILES', '$(call rwildcard, $(SRC), *.cpp)'),
  MakefileVar.new('SRC_OBJ_FILES', '$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRC_FILES))'),
  MakefileVar.new('SRC_DEP_FILES', '$(patsubst $(SRC)/%.cpp, $(OBJ)/%.d, $(SRC_FILES))'),
  MakefileVar.new('SPEC_FILES', '$(call rwildcard, $(SPEC), *.spec.cpp)'),
  MakefileVar.new('SPEC_OBJ_FILES', '$(patsubst $(SPEC)/%.spec.cpp, $(OBJ)/%.spec.o, $(SPEC_FILES))'),
  MakefileVar.new('SPEC_DEP_FILES', '$(patsubst $(SPEC)/%.spec.cpp, $(OBJ)/%.spec.d, $(SPEC_FILES))'),
  MakefileVar.new('PCH', options.pch),
  MakefileVar.new('DEPENDENCIES', '$(SRC_DEP_FILES) $(SPEC_DEP_FILES) ', options.pch ? "$(OBJ)/$(PCH).d" : '' ),
  MakefileVar.new('OBJECTS', '$(SRC_OBJ_FILES) $(SPEC_OBJ_FILES)'),
  MakefileVar.new('EXECUTABLES', '$(BIN)/$(EXE) $(BIN)/$(EXE_SPEC)')
]

phony_targets = [
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
    '-rm -f $(EXECUTABLES) $(OBJECTS) $(DEPENDENCIES) ' + (options.pch ? "$(INCLUDE)/$(PCH).gch" : '')
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

unless options.pch.nil?
  phony_targets.push(MakefileTarget.new(
    true,
    'clear-pch', nil,
    'rm $(INCLUDE)/$(PCH).gch' 
  ))
end

unless options.install.nil?
  phony_targets.push(MakefileTarget.new(
    true,
    'install', nil,
    "@echo 'Installing into $(INSTALL_DIR)",
    "@install $(BIN)/$(EXE) $(INSTALL_DIR)"
  ))
end

real_targets = [
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
    '$(OBJ)/$(MAIN_OBJ)', ['$(SRC)/$(MAIN_CXX) ', options.pch ? "$(INCLUDE)/$(PCH).gch" : ''],
    '$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS) $<'
  ),
  MakefileTarget.new(
    false, 
    '$(OBJ)/%.o',  ['$(SRC)/%.cpp Makefile ', options.pch ? "$(INCLUDE)/$(PCH).gch" : ''],
    '$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS) $<'
  ),
  MakefileTarget.new(
    false,
    '$(OBJ)/%.spec.o', ['$(SPEC)/%.spec.cpp Makefile ', options.pch ? "$(INCLUDE)/$(PCH).gch" : ''],
    '$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP $(INCLUDE_DIRS_SPEC) $<'
  )
]

unless options.pch.nil?
  real_targets.push(MakefileTarget.new(
    false,
    '$(INCLUDE)/$(PCH).gch', ['$(INCLUDE)/$(PCH) Makefile'],
    '$(CXX) $(CXX_FLAGS) -o $@ -c -MMD -MP -x c++-header $(INCLUDE_DIRS) $<'
  ))
end

real_targets += DIRS.map{ |dir| MakefileTarget.new(
  false,
  dir, nil,
  '@mkdir -p $@',
)}

renderer = nil
File.open("#{__dir__}/cpp-repo-files/template.makefile") do |file|
  renderer = ERB.new(file.read, nil, '-')
end

##############
### Output ###
##############

File.open("Makefile", "w") do |file|
  file.write(renderer.result())
end

puts "Done!"

