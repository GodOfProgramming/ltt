#!/usr/bin/env ruby
require 'set'

if ARGV.size != 2
  puts 'Usage: ruby story_finder.rb [input-file] [output-file]'
  exit
end

#regex = /[\s\S]*[\w][\s]*[0-9]*[\s\S]*/
# regex = /[0-9]*/
# regex = /(?![a-z])[0-9]+/
regex = /[0-9]{4}/
stories = Set.new

File.foreach(ARGV[0]) do |line|
  story = line[regex]

  if story && story[0] == '4' && story[1] == '3'
    stories << story 
  end
end

stories = stories.sort

outfile = File.open ARGV[1], 'w'
stories.each do |file|
  outfile.puts file
end
outfile.close
