#!/usr/bin/env ruby
require 'rake/clean'
require 'sys_helpers'

CLEAN.include('*.o')
CLOBBER.include('*.exe')

class CPP
  include Rake::DSL

  attr_accessor :cxx, :exe, :exe_spec, :source_files, :object_files, :includes, :flags, :shared_libs

  def initialize
    @cxx = 'g++'
    @exe = "bin/#{SysHelpers.this_dir}.exe"
    @exe_spec = exe.sub(/\.exe$/, '.spec')

    @source_files = Rake::FileList["src/**/*.cpp"]
    @object_files = source_files.ext('.o').map { |file| file.sub(/^src\//, 'obj/') }

    @includes = Array.new
    @flags = Array.new
    @shared_libs = Array.new

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
    rule('.o' => [proc { |tn| tn.sub(/\.o$/, '.cpp').sub(/^obj\//, 'src/') }]) do |task|
      FileUtils.mkdir_p(File.dirname(task.name))
      sh "#{cxx} #{flags.join(' ')} #{generate_includes} -c #{task.source} -o #{task.name}"
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
