#!/usr/bin/env ruby

if ARGV.length == 0
  puts "Usage: pascal-to-snake.rb [files...]"
  exit
end

require 'rails'

cur_dir = Dir.pwd

ARGV.each do |arg|
    orig = "#{cur_dir}/#{arg}"
    new = "#{cur_dir}/#{arg.underscore}"
    if File.file?(orig)
	puts "renaming #{orig} -> #{new}"
	File.rename(orig, new)
    end
end

