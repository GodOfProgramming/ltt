class SysHelpers
    def self.this_dir
			File.basename(Dir.getwd)
    end

    def self.dirs_under(dir)
      dirs = Dir.glob("#{dir}/**/")
      dirs = dirs.map do |dir|
        dir.split('/').join('/')
      end 
    end
end
