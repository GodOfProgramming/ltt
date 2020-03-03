require 'active_support/core_ext/numeric/conversions'

class Numeric
	def pretty
		self.to_s(:delimited)
	end
end

