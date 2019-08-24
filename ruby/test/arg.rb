#!/bin/ruby

cxx = ARGV[0]
args = ARGV.slice(1..-1)

flags = []
includes = []
lib_dirs = []
static_libs = []
dynamic_libs = []
files = []

i = 0
while i < args
  arg = args[i]

  case arg
  when '-f'
    flags.push(args[i += 1])
  when '-i'
    includes.push(args[i += 1])
  when '-l'
    lib_dirs.push(args[i += 1])
  when '-s'
    static_libs.push(args[i += 1])
  when '-d'
    dynamic_libs.push(args[i += 1])
  else
    files.push(arg)
  end

  i += 1
end

left = ""

flags.each do |f|
  left += " #{f}"
end

includes.each do |i|
  left += " #{i}"
end

right = ""

lib_dirs.each do |l|
  right += " #{l}"
end

static_libs.each do |s|
  right += " #{s}"
end

dynamic_libs.each do |d|
  right += " #{d}"
end

exes = []

files.each do |f|
  fname = File.basename(f, File.extname(f))
  exes.push("#{cxx} -o #{fname}.o #{left} #{f} #{right}")
end

puts exes

