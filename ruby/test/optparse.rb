#!/bin/ruby
require 'optparse'

OptionParser.new do |opts|
    opts.banner = "This is a test script"

    opts.on('-a [ARG]', '--arg', "an argument") do |v|
	puts("a = #{v}")
    end

    opts.on('-b', '--barg [BARG]', "another argument") do |v|
	puts("b = #{v}")
    end

    opts.on('-h', '--hell', 'something') do |v|
    end
end.parse!
