LOOPS = 1000

arr1 = [];
arr2 = [];

for i in 0...100_000 do
  arr1.push(i)
  arr2.push(i)
end

t1 = Time.now
for i in 0...LOOPS do
  foo = arr1.last
end
t2 = Time.now

puts "Access time (last): #{((t2 - t1) / LOOPS.to_f).round(7)}"

t1 = Time.now
for i in 0...LOOPS do
  foo = arr2.reverse.first
end
t2 = Time.now

puts "Access time (reverse.first): #{((t2 - t1) / LOOPS.to_f).round(7)}"

