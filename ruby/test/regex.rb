r = /[\s\S]*[0-9]+[\s\S]*/
f = File.open("test.txt")

f.each_line do |line|
    if !line[r].nil?
	puts line
    end
end
