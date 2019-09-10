class Task
    @vars = {}

    # Static methods
    def self.vars; @vars; end
    def self.define_var(sym, &block)
	vars[sym] = block
    end

    # Instance methods
    def method_missing(name)
	blk = self.class.vars[name]
	unless blk.nil?
	    define_singleton_method(name.to_sym, block)
	else
	    puts "Undefined method: #{name}"
	end
    end

    def initialize(sym, &block)
	puts "Running #{sym}"
	yield if block_given?
    end
end

class Test
    def method_missing(sym, &block)
	puts(sym)
    end
    
    def initialize(sym, &block)
	puts "Testing"
	yield if block_given?
    end
end

def let(sym, &block)

end

def method_missing(name, &block)
    puts name
end

Test.new :foo do
    undefined_var
end



