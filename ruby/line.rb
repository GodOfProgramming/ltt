10.times do |x|
  STDOUT.write "\33[2K\rNumber: #{x}"
  sleep(1)
  STDOUT.write "\33[2K\rzzzzzzzzzzzzzzzzzz"
  sleep(1)
end

puts
