#!/bin/ruby
eval ENV['REQUIRE_ONEOFFS']
require 'optparse'
require 'json'
require 'string'
require 'set'

RUNNING_RELAYS_FILENAME = "#{ENV['HOME']}/memtmp/running-relays.json"

options = {}

OptionParser.new do |opts|
    opts.banner = 'Usage: ruby relay-start.rb [options] [port range begin] [port range end]'

    opts.on("-k", "--kill", "Kill all running relay instances") do |v|
	options[:kill] = v
    end

    opts.on("-i", "--info", "Display all running process IDs") do |v|
	options[:info] = v
    end

    opts.on("-c", "--clear", "Clear the running processes cache") do |v|
	options[:clear] = v
    end
end.parse!

if ARGV.empty? && options.empty?
    puts "Fatal error: you must supply arguments, use -h for help"
end

relay_pids = Set.new

if File.exists? RUNNING_RELAYS_FILENAME
    File.open RUNNING_RELAYS_FILENAME do |file|
	json = file.read
	arr = JSON.parse json
	relay_pids = arr.to_set
    end
end

if options[:kill]
    for relay_pid in relay_pids do
	puts "Killing #{relay_pid}"
	Process.kill('KILL', relay_pid)
	relay_pids.delete relay_pid
    end
elsif options[:info]
    puts "Current number of running relays: #{relay_pids.length}"
    exit
elsif options[:clear]
    puts "Clearing #{relay_pids.length} entries"
    relay_pids.clear
else
    start_port = ARGV[0]
    end_port = start_port

    if ARGV.length == 2
	end_port = ARGV[1]
    end

    unless start_port.numeric? && end_port.numeric?
	puts "Fatal error: port range must be numbers"
	exit
    end

    start_port = start_port.to_i
    end_port = end_port.to_i

    puts "Number of relays to initialize: #{end_port - start_port + 1}"

    for port in start_port..end_port do
	puts "Adding relay 127.0.0.1:#{port}"
	pid = fork { exec("RELAY_ADDRESS='127.0.0.1:#{port}' dist/relay") }
	#pid = fork { exec("sleep 60") }
	puts "Started relay with pid: #{pid}"
	relay_pids.add(pid)
    end
end

File.open RUNNING_RELAYS_FILENAME, "w" do |file|
    file.write relay_pids.to_a.to_json
end

