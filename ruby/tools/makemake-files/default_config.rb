require 'sys_helpers'

$CONFIG = {
  exe: SysHelpers.this_dir,
  cxx: 'g++'
  cxx_flags: [
    '-Wall', 
    '-Wextra', 
    '-std=c++17', 
    '-O3', 
    '-march=native', 
    '-frename-registers', 
    '-funroll-loops'
  ],
  include: [
    'include'
  ],
  include_spec: [
    ENV['CSPEC_INCLUDE']
  ],
  static_libs: [],
  static_libs_spec: [],
  shared_libs: [],
  shared_libs_spec: [
    'cspec'
  ],
  library_dirs: [],
  library_dirs_spec: [],
  use_ltt: false,
  precompiled_header: nil, # set to string to specy the pch file to use
  install_dir: nil # set to a string specifying the install directory
}
