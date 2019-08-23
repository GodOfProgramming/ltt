#!/bin/ruby

if ARGV.length == 0
  puts "Usage: pascal-to-snake.rb [files...]"
  exit
end

require 'rails'

ARGV.each do |fn|
  File.rename(File.new(fn), fn.underscore) if File.file?(fn)
end

