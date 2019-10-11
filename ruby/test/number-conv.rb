number = ARGV[0]

regex = /[0-9]*\.[0-9]*/

if number.match?(regex)
    parts = number.split('.')
    left = parts.first.reverse
    right = parts.last

    left_num = 0
    right_num = 0

    exp = 1
    left.each_char do |char|
	left_num += char.to_i * exp
	exp *= 10
    end

    exp = 0.1
    right.each_char do |char|
	right_num += char.to_i * exp
	exp /= 10.0
    end

    puts left_num + right_num
else
    puts "NaN"
end

