class MakefileTarget
  attr_reader :name, :task, :phony, :deps

  def initialize(name, task, phony, *deps)
    @name = name
    @task = task
    @phony = phony
    @deps = deps
  end

  def to_s
    str = ''
    str += ".PHONY: #{name}\n" if phony
    str += "#{name}: #{deps.join(' ')}\n"
    str += "\t#{task}\n" if task
  end
end
