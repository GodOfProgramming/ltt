function extend(parent, child)
	for k, v in pairs(parent) do
		if (type(v) == 'table') then
			child[k] = merge(parent[k], child[k])
		else
			child[k] = v
		end
	end
	return child
end
