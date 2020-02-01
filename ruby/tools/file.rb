#!/bin/ruby

str = nil
File.open("#{__dir__}/dir.rb") do |file|
    str = file.read
end

puts str
