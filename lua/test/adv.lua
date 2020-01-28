function concat(...)
	return {...} 
end

x = concat(1, 'a', 'b', 'c')

for i, v in ipairs(x) do
	print(i, v)
end

str = [[test string]]

print(str)

foo = "foo"
print("test: ", string.len(foo))
