require 'rails'

ARGV.each do |fn|
  File.rename(File.new(fn), fn.underscore) if File.file?(fn)
end

