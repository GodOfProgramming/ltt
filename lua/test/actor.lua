require 'object'

-- meta class
Actor = extend(Object, {
	name = nil,
	xpos = 0,
	ypos = 0
})

-- initializer

function Actor:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

-- public methods

function Actor:move(x, y)
	self.xpos = self.xpos + x
	self.ypos = self.ypos + y
end

function Actor:to_string()
	return string.format("%s at (%d, %d)", self.name, self.xpos, self.ypos)
end
