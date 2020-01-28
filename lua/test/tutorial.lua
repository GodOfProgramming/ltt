-- also a comment
--[[ This is a comment --]]
print("Defining a & b")
local a, b
a = 10
b = 20

print("a = ", a, "\nb = ", b)

a, b = b, a

print("a = ", a, "\nb = ", b)

c = 30

a, b, c = b, a, 2.5 % 5

print("a = ", a, "\nb = ", b, "\nc = ", c)

x = type(a)

print(x.class)
