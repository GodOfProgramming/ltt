require 'sys_helpers'

$CONFIG = {
  exe: SysHelpers.this_dir,
  cxx: 'g++',
  cxx_flags: [
    "-Wall", 
    "-Wextra", 
    "-std=c++17", 
    "-O3", 
    "-march=native", 
    "-frename-registers", 
    "-funroll-loops"
  ],
  include_dirs: ['$(SRC)', '$(INCLUDE)', '$(RAPIDXML_INCLUDE)'],
  include_dirs_spec: ['$(SRC)', '$(INCLUDE)'],
  static_libs: [],
  static_libs_spec: [],
  shared_libs: [],
  shared_libs_spec: ['cspec'],
  library_dirs: [],
  library_dirs_spec: [],
  use_ltt: nil,
  install_dir: nil,
  precompiled_header: nil
}

