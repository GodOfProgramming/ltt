#!/bin/ruby
require 'optparse'
require 'json'

options = {}

OptionParser.new do |opts|
    opts.banner = 'Usage: ruby relay-start.rb [options] [port range begin] [port range end]'

    opts.on("-k", "--kill", "Kill all running relay instances") do |v|
	options[:kill] = v
    end
end.parse!

home = ENV['HOME'] || ''

running_relays_filename = "#{home}/memtmp/running-relays.json"

running_relays = []

if File.exists? running_relays_filename 
    File.open running_relays_filename do |file|
	json = file.read
	running_relays = JSON.parse json
    end
end

puts "Current number of running relays: #{running_relays.length}"

if options[:kill]
    for relay in running_relays do
	puts `kill #{relay}`
	puts "killed pid #{relay}"
    end
else
    start_port = ARGV[0].to_i
    end_port = ARGV[1].to_i

    for port in start_port..end_port do
	puts "Added relay using port number #{port}"
	puts `RELAY_ADDRESS="127.0.0.1:#{port}" make dev-relay-v2 &`
	pid = `$!`
	puts "Storing pid #{pid}"
	running_relays.push(pid)
    end
end

File.open running_relays_filename, "w" do |file|
    file.write running_relays.to_json
end

