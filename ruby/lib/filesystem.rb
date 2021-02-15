# frozen_string_literal: true

module FileSystem
  def self.traverse(dir, ext, &blk)
    Dir.foreach(dir) do |fn|
      next if ['.', '..'].include?(fn)

      path = "#{dir}/#{fn}"
      if File.directory?(fn)
        each_file(path, ext, &blk)
      else
        next unless File.extname(fn) == ext

        yield(path) if block_given?
      end
    end
  end
end
