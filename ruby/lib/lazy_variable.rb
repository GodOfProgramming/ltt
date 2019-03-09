class LazyVariable 
	def set(&blk)
		@value_setter = blk
	end

	def get
		@value ||= @value_setter && @value_setter.call 
	end
end
