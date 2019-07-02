if ARGV.length == 0 
  puts 'need end number'
  exit
end

start = 1
stop = ARGV[0].to_i

for i in start..stop
  half = i / 2
  prime = true 
  for j in 2..half
    prime = false if i % j == 0
  end
  puts i if prime
end

