#!/bin/ruby

if ARGV.length == 0
  puts "Usage: snake-to-pascal.rb [files...]"
  exit
end

require 'rails'

ARGV.each do |fn|
  File.rename(File.new(fn), fn.camelize) if File.file?(fn)
end

