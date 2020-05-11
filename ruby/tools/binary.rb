#!/usr/bin/env ruby

count = 0
ARGV.each_with_index do |val, indx|
	count |= (val.to_i(16) << (indx * 8))
end
puts count

