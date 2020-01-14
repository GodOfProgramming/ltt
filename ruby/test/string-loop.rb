def make_class(num)
    klass = "Class#{num}"
    <<-CPP
    class #{klass} {
     public:
      #{klass}() = default;
    };
    CPP
end

def text(nums)
    output = ""

    nums.each do |num|
	output = "#{output} #{make_class(num)}"
    end

    output
end

arr = [1, 2, 3, 4, 5]

str = <<-STR
#pragma once

namespace something { 
    class SomethingElse {}; 
    #{ text(arr) } 
}
STR

puts str

