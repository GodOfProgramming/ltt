#!/bin/ruby

class Fixnum
  N_BYTES = [42].pack('i').size
  N_BITS = N_BYTES * 8
  MAX = 2 ** (N_BITS - 2) - 1
  MIN = -MAX - 1
end

if ARGV.length == 0 
  puts 'need number of primes'
  exit
end

num_primes = ARGV[1]
range_begin = ENV['prime_start']
range_end = ENV['prime_end']

start = range_begin ? range_begin.to_i : 1
stop = range_end ? range_end.to_i : Fixnum::MAX

i = start
count = 0
while i < stop && count < num_primes do
  half = i / 2

  for j in 2..half
    if i % j == 0
      puts i
      count += 1 
      break
    end
  end

  i++
end

