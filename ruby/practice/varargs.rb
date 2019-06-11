def foo(*args)
  args.each do |a|
    puts a
  end
end

def bar(single, *many)
  many.each do
    puts single
  end
end

bar(*ARGV)
