#!/usr/bin/env ruby

cxx = ARGV[0]
args = ARGV.slice(1..-1)

flags = []
includes = []
lib_dirs = []
static_libs = []
dynamic_libs = []
files = []

i = 0
while i < args.length
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
  left += " -I#{i}"
end

middle = ""

lib_dirs.each do |l|
  middle += " -L#{l}"
end

static_libs.each do |s|
  middle += " #{s}"
end

right = ""

dynamic_libs.each do |d|
  right += " #{d}"
end

exes = []

files.each do |f|
  fname = File.basename(f, File.extname(f))
  exes.push("#{cxx}#{left} #{f}#{middle} -o out/#{fname}.o")
end

class Pair
  attr_accessor :first, :second
  def initialize(f, s)
    first = f
    second = s
  end
end

threads = []
pairs = Array.new(exes.length)

semaphor = Mutex.new

i = 0
while i < exes.length
  t = Thread.new(i) do |id|
    puts "Command #{id}: #{exes[id]}"
    res = `#{exes[id]}`
    semaphor.lock
    pairs.push(Pair.new(files[id], res))
    semaphor.unlock
  end

  threads.push(t)
  i += 1
end

threads.each do |t|
  t.join
end

pairs.each do |pair|
  if pair
    puts pair.first unless pair.first.nil?
    puts pair.second unless pair.second.nil?
    STDOUT.flush
  end
end

