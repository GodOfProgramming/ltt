#!/bin/ruby
threads = []

i = 0
while i < 5
  t = Thread.new(i) do |id|
    puts id 
  end

  threads.push(t)
  i += 1
end

threads.each do |t|
  t.join
end

