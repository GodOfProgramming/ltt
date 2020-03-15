#!/bin/ruby
require 'optparse'

def print_path
  path = ENV['PATH']
  path.split(':').each do |p|
    puts p
  end
end

OptionParser.new do |opts|
  opts.on('--print-path', 'Pretty print the current path') do |v|
    print_path
  end
end.parse!

