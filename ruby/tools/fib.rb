#!/bin/ruby

# usage: fib.rb [number]

unless ARGV.length > 0
	puts "You need to supply an argument"
	exit 1
end

# convert the first argument to a number
count = ARGV[0].to_i 

if count < 0
	puts "You need to use a positive number"
	exit 1
end

# create a function that computes a iterative fibonnaci sequence
def fib count
  prev = 0
  curr = 1
  for i in 0...count do
    tmp = curr
    curr += prev
    prev = tmp
  end
  prev
end

# call the function and store the result

result = fib count

# print the result to the screen
puts result
