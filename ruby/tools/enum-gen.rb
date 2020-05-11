#!/usr/bin/env ruby

class Options
  class << self
    attr_accessor :infile, :outfile, :enum
  end
end

class ArgHandler
  def self.parse
    check_infile
    check_outfile
    check_enum

    check_required
  end

  private

  def self.check_infile
    Options.infile = get_val(ARGV.index('-i'))
  end

  def self.check_outfile
    Options.outfile = get_val(ARGV.index('-o'))
  end

  def self.check_enum
    Options.enum = get_val(ARGV.index('-e')
  end

  def self.get_val(indx)
    ARGV[indx + 1] if indx != nil && ARGV.length > indx + 1
  end

  def check_required
    if Options.enum == nil
      puts "Enum name required, use -e"
      exit
    end

    if Options.outfile == nil
      puts "Outfile name required, use -o"
      exit
    end
  end
end

ArgHandler.parse

if Options.infile != nil
  # input file present
else
  # get from cmd line
end

header_lines = [];

header_lines.push "enum class #{Options.enum} {"


