#!/bin/ruby
ARGV.each do |a|
  puts File.basename(a, File.extname(a))
end
