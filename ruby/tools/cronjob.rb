#!/usr/bin/env ruby
MEMTMP = ENV['MEMTMP'];
CRONJOB = ARGV[0]
CRONTAB = `crontab -l`
TMPFILE = "#{MEMTMP}/crontmp"

unless CRONTAB.include?(CRONJOB)
  puts "appending cronjob: '#{CRONJOB}'"
  File.open(TMPFILE, 'w') do |file|
    newtab = CRONTAB + CRONJOB + "\n"
    file.write(newtab)
  end
  `crontab #{TMPFILE}`
end

