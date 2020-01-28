require 'extendable'

-- metaclass

Object = {}

-- initializer

function Object:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	self.class = Object
	return o
end

-- public methods

function Object:to_string()
	return self
end

function Object:print()
	print(self:to_string())
end
