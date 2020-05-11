#!/usr/bin/env ruby
require 'set'

if ARGV.size != 2
  puts 'Usage: ruby rails_failure_finder.rb [input-file] [output-file]'
  exit
end

# regex = /(^|\A)(rspec .\/)[\/a-z_]*(.rb:)[0-9]*( #)[\s\S]*/
regex = /(\.\/)[a-z\/_]+(_spec\.rb)/
files = Set.new

File.foreach(ARGV[0]) do |line|
  name = line[regex]

  if name
    files << name[2, name.length]
  end
end

outfile = File.open ARGV[1], 'w'
files.each do |file|
  outfile.puts file
end
outfile.close
