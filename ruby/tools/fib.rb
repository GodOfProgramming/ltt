#!/usr/bin/env ruby

# usage: fib.rb [number]

unless ARGV.length > 0
  puts "You need to supply an argument"
  exit 1
end

# convert the first argument to a number
count = ARGV[0].to_i 
reps = ARGV[1] && ARGV[1].to_i || 1

if count < 0
  puts "You need to use a positive number"
  exit 1
end

def fib n 
  if n <= 1
    n
  else 
    fib(n - 2) + fib(n - 1)
  end 
end

# create a function that computes a iterative fibonnaci sequence
def fib_it count
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

now = Time.new.to_f
for i in 0..reps do
  result = fib count
end
after = Time.new.to_f

# print the result to the screen
puts result

puts "took #{(after - now) / reps.to_f} seconds"

