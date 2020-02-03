#!/bin/ruby
require 'optparse'
require 'ostruct'
require 'sys_helpers'
require 'fileutils'
require 'erb'

#######################
### Option Parsing ####
#######################

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
options.install = false
options.use_ltt = false

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

	opts.on('', "--include [INCLUDE]", "An include dir to use") do |v|
		options.include.push v
	end

	opts.on('', "--include-spec [INCLUDE]", "An include dir to use for specs") do |v|
		options.include_spec.push v
	end

	opts.on('', "--static [LIB]", "A static library to use") do |v|
		options.static.push v
	end

	opts.on('', "--static-spec [LIB]", "A static library to use when building specs") do |v|
		options.static_spec.push v
	end

	opts.on('', "--shared [LIB]", "A shared library to use") do |v|
		options.shared.push v
	end

	opts.on('', "--shared-spec [LIB]", "A shared library to use when building specs") do |v|
		options.shared_spec.push v
	end

	opts.on('', "--library [LIB]", "A library directory") do |v|
		options.library_dirs.push v
	end

	opts.on('', "--library-spec [LIB]", "A library directory when building specs") do |v|
		options.library_dirs_spec.push v
	end

	opts.on('', "--install [DIR]", "Create a target to install to DIR") do |v|
		options.install = v
	end

	opts.on('', "--use-ltt", "Enable the ltt repo libs") do |v|
		options.use_ltt = v
	end
end.parse!

if !options.exe
	puts "-e, --exe is required"
	exit 1
end

if !options.cxx
	puts "-c, --cxx is required"
	exit 1
end

###########################
### Makefile Generation ###
###########################

def derive_autogen_commands(opts)
	str = File.basename(__FILE__)
	str = "#{str} -e '#{opts.exe}'" if opts.exe != SysHelpers.this_dir
	str = "#{str} -c '#{opts.cxx}'" if opts.cxx != 'g++'
	str = "#{str} #{opts.cxx_flags.map{ |f| "-f '#{f}'" }.join(' ')}" unless opts.cxx_flags.empty?
	str = "#{str} #{opts.include.map{ |i| "--include '#{i}'" }.join(' ')}" unless opts.include.empty?
	str = "#{str} #{opts.include_spec.map { |i| "--include-spec '#{i}'" }.join(' ')}" unless opts.include_spec.empty?
	str = "#{str} #{opts.static.map{ |s| "--static '#{s}'" }.join(' ')}" unless opts.static.empty?
	str = "#{str} #{opts.static_spec.map{ |s| "--static-spec '#{s}'" }.join(' ')}" unless opts.static_spec.empty?
	str = "#{str} #{opts.shared.map{ |s| "--shared '#{s}'" }.join(' ')}" unless opts.shared.empty?
	str = "#{str} #{opts.shared_spec.map{ |s| "--shared-spec '#{s}'" }.join(' ')}" unless opts.shared_spec.empty?
	str = "#{str} --install" if opts.install
	str = "#{str} --use-ltt" if opts.use_ltt
	str
end

def use_ltt?(opts)
	'$(MAKE_INCLUDE_HELPER)' if opts.use_ltt
end

DIRS = [ '$(SRC)', '$(INCLUDE)', '$(SPEC)', '$(BIN)', '$(OBJ)', '$(OBJ_DIRS)' ]

setup_dirs = DIRS.join(' ')

renderer = nil
File.open("#{__dir__}/repo-creator-files/makefile-template.erb") do |file|
	renderer = ERB.new(file.read)
end

##############
### Output ###
##############

File.open("Makefile", "w") do |file|
	file.write(renderer.result())
end

FileUtils.cp("#{__dir__}/repo-creator-files/.clang-format", Dir.pwd)

puts "Done!"

