class MakefileVar
  attr_reader :name, :value

  def initialize(name, value)
    @name = name
    @value = value
  end

  def to_s
    "%-20s:= %s" % [name, value]
  end
end


