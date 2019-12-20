#!/usr/bin/ruby

if ARGV.length == 0
  puts "Usage: snake-to-pascal.rb [files...]"
  exit
end

require 'rails'

cur_dir = Dir.pwd

ARGV.each do |arg|
    orig = "#{cur_dir}/#{arg}"
    new = "#{cur_dir}/#{File.basename(arg, File.extname(arg)).camelize}#{File.extname(arg)}"
    puts "renaming #{orig} -> #{new}"
    File.rename(orig, new)
end

