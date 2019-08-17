#!/bin/ruby
require 'set'

if ARGV.size != 3
  puts 'Usage: ruby file_differ.rb [file-a] [file-b] [results]'
  exit
end

regex = /[^:]+/

set_a = Set.new
set_b = Set.new

File.foreach(ARGV[0]) do |line|
  file = line[regex]

  if file
    set_a << file
  end
end

File.foreach(ARGV[1]) do |line|
  file = line[regex]

  if file
    set_b << file
  end
end

diff = set_a - set_b | set_b - set_a

outfile = File.open(ARGV[2], 'w')

diff.each do |file|
  outfile.puts(file)
end

outfile.close
