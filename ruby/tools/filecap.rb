#!/usr/bin/env ruby
require 'rails'

cur_dir = Dir.pwd

ARGV.each do |arg|
    orig = "#{cur_dir}/#{arg}"
    new = "#{cur_dir}/#{File.basename(arg, File.extname(arg)).camelize}#{File.extname(arg)}"
    puts "renaming #{orig} -> #{new}"
    File.rename(orig, new)
end

