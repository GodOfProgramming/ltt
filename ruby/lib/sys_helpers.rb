class SysHelpers
    def self.this_dir
			File.basename(Dir.getwd)
    end
end
