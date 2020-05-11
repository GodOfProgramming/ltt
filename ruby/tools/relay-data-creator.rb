#!/usr/bin/env ruby
require 'optparse'
require 'json'
require 'ostruct'
require 'faker'

# From one-offs
require 'string'
require 'process'

unless ENV.has_key?('RELAY_DEBUG_FILENAME')
    puts "Set the env var: RELAY_DEBUG_FILENAME"
    exit 1
end

RELAY_DATA_FILENAME = ENV['RELAY_DEBUG_FILENAME']

unless !RELAY_DATA_FILENAME.nil?
    puts "give the env var RELAY_DEBUG_FILENAME a value"
    exit 1
end

options = OpenStruct.new
options.starting_address = '127.0.0.1:20000'
options.number_of_entries = 0
options.info = false

OptionParser.new do |opts|
    opts.banner = 'Usage: ruby relay-data-creator.rb [options]'

    opts.on('-a', '--address [ADDRESS]', 'Specify the starting address') do |v|
	options.starting_address = v
    end

    opts.on('-n', '--number [NUMBER]', 'Specify the number of entries to create') do |v|
	options.number_of_entries = v.to_i if v.numeric?
    end

    opts.on('-i', '--info', 'Analyze the file and give detailed information') do |v|
	options.info = v
    end
end.parse!

data = {}

parts = options.starting_address.split('.')
pair = parts.last.split(':')
parts.pop
first_part = parts.join('.')

current_address = pair.first.to_i
current_port = pair.last.to_i
for i in 0...options.number_of_entries do
    addr = "#{first_part}.#{current_address}:#{current_port}"
    data[addr] = {
	latitude: Faker::Address.latitude,
	longitude: Faker::Address.longitude,
	datacenter_name: "#{Faker::Address.country}:#{Faker::Address.state}:#{Faker::Address.city}"
    }

    current_address += 1
    current_port += 1 
end

output = JSON.pretty_generate data

puts output

unless data.empty?
    File.open(RELAY_DATA_FILENAME, 'w') do |file|
	file.write(output)
    end
end

