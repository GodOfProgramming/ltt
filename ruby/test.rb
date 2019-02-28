class Foo
  def initialize
  end

  def method
    { part1: "part1", part2: "part2" }
  end
end

class Bar < Foo
  def initialize
  end

  def method
    super.merge({ part2: "" })
  end
end

x = Foo.new
y = Bar.new

puts x.method
puts y.method

