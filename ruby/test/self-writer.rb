class FuncGenerator
    def method_missing(name, *args, &blk)
	puts "Method #{name} missing, generating file..."
	file = File.open("#{name}.rb", "w")

	file << <<-RUBY
	class FuncGenerator
	    def #{name}
		puts("Calling #{name} from #{name}.rb")
	    end
	end
	RUBY

	file.close

	require_relative "#{name}.rb"
    end
end

g = FuncGenerator.new

g.some_method

g.some_method

