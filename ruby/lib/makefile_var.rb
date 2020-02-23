class MakefileVar
  attr_reader :name, :values

  def initialize(name, *values)
    @name = name
    @values = values
  end

  def to_s
    "%-20s:= %s\n" % [name, values.join(' ')]
  end
end


