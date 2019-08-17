def thing(foo, bar)
  case
  when !foo && !bar
    "foobar"
  when foo
    "foo"
  when bar
    "bar"
  end
end

puts thing(true, false)
