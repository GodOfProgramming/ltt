#!/usr/bin/env ruby
require 'rake/clean'
require 'rake/loaders/makefile'
require 'sys_helpers'

CLEAN.include('**/*.o', '**/*.mf')
CLOBBER.include('**/*.exe')

class CPP
  include Rake::DSL

  module MyLibs
    Art = 'art'
    Utility = 'utility'
  end

  attr_accessor :cxx, :exe, :exe_spec, :source_files, :object_files, :dependency_files, :includes, :flags, :shared_libs, :my_libs

  def initialize
    @cxx = 'g++'
    @exe = "bin/#{SysHelpers.this_dir}.exe"
    @exe_spec = exe.sub(/\.exe$/, '.spec')

    @source_files = Rake::FileList["src/**/*.cpp"]
    @object_files = source_files.ext('.o').map { |file| file.sub(/^src\//, 'obj/') }
    @dependency_files = source_files.ext('.mf').map { |file| file.sub(/^src\//, 'obj/') }

    @includes = Array.new
    @flags = Array.new
    @shared_libs = Array.new
    
    lib_dir = ENV['CPP_LIB']
    @my_libs = {
      MyLibs::Art => "#{lib_dir}/art/include",
      MyLibs::Utility => "#{lib_dir}/utility/include",
    }

    # create the repo init task
    task :init do
      FileUtils.mkdir_p('src')
    end

    task :ycm do
      FileUtils.cp("#{__dir__}/.ycm_extra_conf.py", Dir.pwd)
    end

    task :clang_format do
      FileUtils.cp("#{__dir__}/.clang-format", Dir.pwd)
    end
  end

  def enable_default_task
    task :default => exe

    desc "Build the binary executable"
    file exe => object_files do |task|
      FileUtils.mkdir_p(File.dirname(task.name))
      sh "#{cxx} #{flags.join(' ')} #{object_files.join(' ')} -o #{task.name} #{generate_shared_libs}"
    end
  end

  def enable_obj_rule
    rule('.o' => source_files) do |task|
      FileUtils.mkdir_p(File.dirname(task.name))
      sh "#{cxx} #{flags.join(' ')} #{generate_includes} -c #{task.source} -o #{task.name}"
    end
  end

  def enable_dependency_generation
    rule('.mf' => source_files) do |task|
      FileUtils.mkdir_p(File.dirname(task.name))
      sh "#{cxx} #{generate_includes} -MM #{task.source} -MP -MF #{task.name}"
    end

    for dep in dependency_files do
      file(dep => source_files)
      import(dep)
    end
  end

  def enable_my_libs(*new_my_libs)
    for lib in new_my_libs do
      includes.push(my_libs[lib])
    end
  end

  def add_include(*new_includes)
    includes.concat(new_includes)
  end

  def add_flag(*new_flags)
    flags.concat(new_flags)
  end

  def add_shared_lib(*new_shared_libs)
    shared_libs.concat(new_shared_libs)
  end

  def defaults!
    enable_default_task
    enable_obj_rule
    enable_dependency_generation 
    add_default_flags
    add_default_includes
  end

  def add_default_includes
    add_include('include')
  end

  def add_default_flags
    add_flag('-Wall', '-Wextra', '-std=c++17', '-O3', '-march=native', '-frename-registers', '-funroll-loops')
  end

  private

  def generate_includes
    includes.map { |i| "-I#{i}" }.join(' ')
  end

  def generate_shared_libs
    shared_libs.map { |l| "-l#{l}" }.join(' ')
  end
end
