#!/usr/bin/ruby

str = "keep\nkeep\nremove\nremove\nremove\nkeep\nkeep\n"

puts str.lines.to_a[2...4].join

