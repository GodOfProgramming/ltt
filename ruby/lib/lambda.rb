class Lambda 
	def method_missing(m, *args, &blk)
		self.define_singleton_method(m, blk)
	end
end
