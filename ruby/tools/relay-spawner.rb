#!/bin/ruby
require 'optparse'
require 'json'
require 'set'
require 'ostruct'

# From one-offs
require 'string'
require 'process'

RELAY_PIDS_FILENAME = "#{ENV['MEMTMP']}/running-relays.txt"

options = OpenStruct.new
options.kill = false
options.clear = false
options.list = false
options.reset = false
options.end = nil
options.type = nil
options.id = nil
options.single_relay = false
options.number = nil

OptionParser.new do |opts|
    opts.banner = 'Usage: ruby relay-start.rb [options] [port range begin] [port range end]'

    opts.on("-k", "--kill", "Kill all running relay instances") do |v|
	options.kill = v
    end

    opts.on("-c", "--clear", "Clear the running processes cache") do |v|
	options.clear = v
    end

    opts.on("-l", "--list", "List all the relay process ids") do |v|
	options.list = v
    end

    opts.on("-r", "--reset", "Reset Redis before operations") do |v|
	options.reset = v
    end
    
    opts.on("-t", "--type [TYPE]", "Specify the type the id is") do |v|
	options.type = v
    end

    opts.on("-i", "--id [ID]", "Specify the id to use when modifying the relay") do |v|
	options.id = v.to_i if v.numeric?
    end

    opts.on("-n", "--number [NUMBER]", "Specify the number of relays to spawn, starting from the given port") do |v|
	options.number = v.to_i if v.numeric?
    end
end.parse!

if (options.type.nil? && !options.id.nil?) || (!options.type.nil? && options.id.nil?)
    puts "Error: Must use both -i & -t options at the same time with values"
elsif !options.type.nil? && !options.id.nil?
    options.single_relay = true
end

if ARGV.empty? && options.empty?
    puts "Error: you must supply arguments, use -h for help"
end

relay_pids = Set.new

if File.exists? RELAY_PIDS_FILENAME
    File.foreach(RELAY_PIDS_FILENAME) do |line|
	relay_pids.add(line.to_i) if line.numeric?
    end
end

puts "Clearing Redis: #{`redis-cli FLUSHALL`}" if options.reset

def for_each_pid(pids, &blk)
    for pid in pids do
	if block_given?
	    yield pid
	end
    end
end

def safe_delete_file
    File.delete RELAY_PIDS_FILENAME if File.exists? RELAY_PIDS_FILENAME
end

if options.single_relay
    if options.type == "pid"
	if options.kill
	    puts "Killing #{options.id}"
	    # Kill the process
	    if check_pid(options.id)
		Process.kill('KILL', options.id) 
	    else
		puts "Error: process #{options.id} does not exist"
	    end
	    # Remove it from the set of running pids
	    relay_pids.delete(options.id)
	    # Create the file and add all the remaining pids to it
	    File.open(RELAY_PIDS_FILENAME, "w") do |file|
		for_each_pid(relay_pids) do |pid|
		    puts "Writing #{pid} to file"
		    file.write("#{pid}")
		end
	    end
	end
    end
elsif options.kill
    for_each_pid(relay_pids) do |pid|
	puts "Killing #{pid}"
	if check_pid(pid)
	    Process.kill('KILL', pid) 
	else
	    puts "Error: process #{pid} does not exist"
	end
    end 
    safe_delete_file
elsif options.clear
    puts "Clearing #{relay_pids.length} entries"
    safe_delete_file
elsif options.list
    puts "Listing realy processes"
    for_each_pid(relay_pids) do |pid|
	puts pid
    end
elsif ARGV.length > 0
    start_port = ARGV[0].numeric? ? ARGV[0].to_i : nil
    end_port = start_port

    if start_port.nil?
	puts "Error: ports must be a numbers"
	exit
    end

    if !options.number.nil?
	end_port = start_port + options.number - 1
    elsif ARGV.length == 2
	end_port = ARGV[1].to_i if ARGV[1].numeric?
    end

    if start_port > end_port
	puts "Error: ending port must be greater than starting"
	exit
    end

    puts "Number of relays to initialize: #{end_port - start_port + 1}"

    for port in start_port..end_port do
	puts "Adding relay 127.0.0.1:#{port}"
	command_str = "RELAY_ADDRESS='127.0.0.1:#{port}' $(pwd)/cmd/relay/bin/relay > /dev/null & echo $! >> #{RELAY_PIDS_FILENAME}"
	system("/bin/bash -c '#{command_str}'")
    end
end

