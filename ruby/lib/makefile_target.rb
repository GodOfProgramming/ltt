class MakefileTarget
  attr_reader :name, :tasks, :phony, :deps

  def initialize(phony, name, deps, *tasks)
    @phony = phony
    @name = name
    @deps = deps
    @tasks = tasks
  end

  def to_s
    str = ''
    str += ".PHONY: #{name}\n" if phony
    str += "#{name}: #{ if deps; deps.join(' '); end }\n"
    str += "\t#{ tasks.map{ |task| "#{task}\n" }.join("\t") }" if tasks.length > 0
    str
  end
end
