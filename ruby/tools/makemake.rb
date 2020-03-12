#!/bin/ruby
require 'optparse'
require 'ostruct'
require 'fileutils'
require 'erb'
require_relative 'makemake-files/makefile_vars.rb'
require_relative 'makemake-files/makefile_targets.rb'

######################
### Option Parsing ###
######################

options = OpenStruct.new
options.config_file = 'makemake.rb'
options.install_ycm = false
options.install_clang_format = false
options.create_default = false

OptionParser.new() do |opts|
  opts.on('-c', '--config [FILE]', 'Specify the config file to use, defaults to "makemake.rb"') do |v|
    options.config_file = v
  end

  opts.on('-d', '--default', 'Create the default config file in this directory') do |v|
    options.create_default = v
  end

  opts.on("--ycm", "Copy the YouCompleteMe extra conf file") do |v|
    options.install_ycm = v
  end

  opts.on("--clang-format", "Copy the clang-format file") do |v|
    options.install_clang_format = v
  end
end.parse!

if options.create_default
  FileUtils.cp("#{__dir__}/makemake-files/makemake.rb", Dir.pwd)
  exit 0
end

if options.install_ycm
  FileUtils.cp("#{__dir__}/makemake-files/.ycm_extra_conf.py", Dir.pwd)
  exit 0
end

if options.install_clang_format
  FileUtils.cp("#{__dir__}/makemake-files/.clang-format", Dir.pwd)
  exit 0
end

###########################
### Makefile Generation ###
###########################

class Config
  attr_reader :exe, 
    :cxx, :cxx_flags, 
    :include_dirs, :include_dirs_spec,
    :static_libs, :static_libs_spec,
    :shared_libs, :shared_libs_spec,
    :library_dirs, :library_dirs_spec,
    :use_ltt, :precompiled_header,
    :install_dir

  def initialize(config = {})
    @exe = config[:exe] || nil
    @cxx = config[:cxx] || nil
    @cxx_flags = config[:cxx_flags] || []
    @include_dirs = config[:include_dirs] || []
    @include_dirs_spec = config[:include_dirs_spec] || []
    @static_libs = config[:static_libs] || []
    @static_libs_spec = config[:static_libs_spec] || []
    @shared_libs = config[:shared_libs] || []
    @shared_libs_spec = config[:shared_libs_spec] || []
    @library_dirs = config[:library_dirs] || []
    @library_dirs_spec = config[:library_dirs_spec] || []
    @use_ltt = config[:use_ltt] || false
    @use_precompiled_header = config[:precompiled_header] || nil
    @create_install_target = config[:install_dir] || nil
  end
end

config_file = "#{Dir.pwd}/#{options.config_file}"

unless File.exist? config_file
  puts "Could not open #{options.config_file} for reading"
  exit 1
end

puts "Using #{options.config_file} for configuration"

config_str = nil

File.open(config_file) do |file|
  config_str = file.read
end

if config_str.empty?
  puts "Config file is empty"
  exit 1
end

eval(config_str)

unless defined?($CONFIG)
  puts "Variable '$CONFIG' is not defined"
  exit 1
end

puts "Successfully read config file"

config = Config.new($CONFIG)

if config.exe.nil? || config.exe.empty?
  puts "exe property in config is required"
  exit 1
end

if config.cxx.nil? || config.cxx.empty?
  puts "cxx property in config is required"
  exit 1
end

if !config.precompiled_header.nil? && config.precompiled_header[/.+\.h/].nil?
  puts "precompiled_header has an invalid file name, please specify a *.h file"
  exit 1
end

vars = MakefileVars.new(config)

targets = MakefileTargets.new(config)

$VARS = vars.all

puts "Created variables"

$PHONY_TARGETS = targets.phony_targets

puts "Created phony targets"

$REAL_TARGETS = targets.real_targets

puts "Created real targets"

renderer = nil

puts "Rendering template"

File.open("#{__dir__}/makemake-files/template.makefile") do |file|
  renderer = ERB.new(file.read, nil, '-')
end

##############
### Output ###
##############

File.open("Makefile", "w") do |file|
  file.write(renderer.result())
end

puts "Done!"

