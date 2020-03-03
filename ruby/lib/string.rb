class String
	def base_10?
		Float(self) != nil rescue false
	end

	# convert to base 16 first (all base 10 work + all base 16) then go back
	def base_16?
		!self[/\H/]
	end

	def numeric?
		base_10? || base_16?
	end
end

