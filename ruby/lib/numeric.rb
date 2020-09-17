require 'active_support/core_ext/numeric/conversions'

class Numeric
	def pretty
		self.to_s(:delimited)
	end

  def self.apply_commas_to(num)
    str = num.to_s.reverse
    str.gsub!("([0-9]{3})","\\1,")
    str.gsub(",$","").reverse
  end
end

