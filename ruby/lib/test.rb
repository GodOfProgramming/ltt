require_relative 'lazy_variable'

x = LazyVariable.new

x.set {
	"something"
}

puts x.get
