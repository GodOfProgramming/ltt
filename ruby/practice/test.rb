class X
  def y
    @y
  end

  def y= a
    @y = a
  end
end

def change x
  x.y = 0
end

x = X.new

x.y = 1

puts "#{x.y}"

change(x)

puts "#{x.y}"
