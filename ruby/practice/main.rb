require "./module.rb"

class Test

  	extend Printer

	def self.name
	  "Test"
	end

end

Test.print
