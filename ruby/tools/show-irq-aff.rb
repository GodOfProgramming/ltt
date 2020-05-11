#!/usr/bin/env ruby
require 'pp'
require 'optparse'
require 'string'
require 'etc'

show_smp_affinity = false
show_affinity_hint = false

OptionParser.new do |opts|
	opts.on('--affinity', 'list the contents of the smp_affinity file') do |v|
		show_smp_affinity = v
	end

	opts.on('--hint', 'list the contents of the affinity_hint file') do |v|
		show_affinity_hint = v
	end
end.parse!

irq_regex = /\/proc\/irq\/[0-9]+/
irqs = Dir.glob('/proc/irq/*').select do |irq_num|
	irq_regex.match? irq_num
end

def expand_proc_mask(num)
	status = []
	for i in 0..Etc.nprocessors do
		status.push("Proc #{i}: #{(num >> i) & 1}")
	end
	status.join("\n\t")
end


if (show_smp_affinity)
	puts ("***** SMP_AFFINITY *****")
	irqs.each do |irq|
		num = irq[/[0-9]+/]
		contents = File.read("#{irq}/smp_affinity").strip
		next unless (contents.base_16?)
		puts "#{num}\n\t#{expand_proc_mask(contents.to_i(16))}"
	end
end

if (show_affinity_hint) 
	puts ("***** AFFINITY_HINT *****")
	irqs.each do |irq|
		num = irq[/[0-9]+/]
		contents = File.read("#{irq}/affinity_hint")
		puts "#{num} -> #{contents}"
	end
end



