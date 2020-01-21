#!/usr/bin/ruby

count = ARGV[0].to_i || 0

def foo count
    prev = 0
    curr = 1
    for i in 0...count do
	tmp = curr
	curr += prev
	prev = tmp
    end
    prev
end

puts(foo(count))
