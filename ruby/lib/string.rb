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

  def self.random(length)
    @charset = [('a'..'z'), ('A'..'Z')].map(&:to_a).flatten
    (0...length).map { @charset[rand(@charset.length)] }.join
  end
end

