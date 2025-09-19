#!/usr/bin/env ruby

require 'optparse'
require 'ostruct'
require 'fileutils'
require 'yaml'

def format_hash(prefix, hash)
  hash.map { |k, v| [ "#{prefix}/#{k}", "#{prefix}/#{v}" ] }.to_h
end

def load_models()
  data = YAML.load(File.read("#{__dir__}/dq8models.yaml"))

  models = {}

  for prefix, model_map in data
    models.merge!(format_hash(prefix, model_map))
  end

  models
end

all = load_models()

options = OpenStruct.new({
  source_dir: ENV['DQ8_SOURCE_DIR'],
  target_dir: ENV['DQ8_TARGET_DIR'],
  from: nil,
  from_variant: nil,
  to: nil,
  to_variant: nil,
  raw: false,
  clear: nil,
})

parser = OptionParser.new() do |opts|
  opts.on('--source <SOURCE>', 'Where to pull the model from') do |v|
    options.source_dir = v
  end

  opts.on('--target <TARGET>', 'The game mods directory') do |v|
    options.target_dir = v
  end

  opts.on('-f', '--from <FROM>', 'The model to swap from') do |v|
    options.from = v
  end

  opts.on('--from-variant <VARIANT>', 'Supply a modifier to the from value') do |v|
    options.from_variant = v
  end

  opts.on('-t', '--to <TO>', 'The model to swap to') do |v|
    options.to = v
  end

  opts.on('--to-variant <VARIANT>', 'Supply a modifier to the to value') do |v|
    options.to_variant = v
  end

  opts.on('-r', '--raw', 'Use a raw file to replace with') do |_|
    options.raw = true
  end

  opts.on('-c', '--clear <TARGET>', 'Clear the target') do |v|
    options.clear = v
  end

  opts.on('--list-keys', 'List all available keys') do |v|
    puts(all.keys.join(' '))
    exit(0)
  end

  opts.on('--list-raw', 'List all raw targets') do |_|
    if options.source_dir
      raw_players = Dir.glob("#{options.source_dir}/player/**").map { |v| "player/#{File.basename(v)}" }
      raw_npcs = Dir.glob("#{options.source_dir}/npc/**").map { |v| "npc/#{File.basename(v)}" }
      raw_monsters = Dir.glob("#{options.source_dir}/monster/**").map { |v| "monster/#{File.basename(v)}" }
      raw_all = raw_players.concat(raw_npcs).concat(raw_monsters)
      puts(raw_all.join(' '))
    end
    exit(0)
  end

  opts.on('--list-options', 'List all available options') do |_|
    puts("--source --target -f --from --from-variant -t --to --to-variant --raw -c --clear")
    exit(0)
  end

  opts.on('--completion-script') do |_|
    immediately_quit = true
    FUNCTION_NAME = "_complete_#{File.basename(__FILE__, File.extname(__FILE__))}"
    THIS_FILE = File.basename(__FILE__)
    puts(
      <<~BASH
        #{FUNCTION_NAME}() {
          COMPREPLY=()

          local curr="${COMP_WORDS[COMP_CWORD]}"
          local prev="${COMP_WORDS[COMP_CWORD-1]}"

          case "${prev}" in
            -f|--from|--clear)
              local keys="$(#{THIS_FILE} --list-keys)"
              COMPREPLY=( $( compgen -W "${keys}" -- "${curr}" ) )
              return 0
              ;;
            -t|--to)
              local keys
              if [[ " ${COMP_WORDS[*]} " =~ [[:space:]]--raw[[:space:]] ]]; then
                keys="$(#{THIS_FILE} --list-raw)"
              else
                keys="$(#{THIS_FILE} --list-keys)"
              fi
              COMPREPLY=( $( compgen -W "${keys}" -- "${curr}" ) )
              return 0
              ;;
          esac

          local opts="$(#{THIS_FILE} --list-options)"
          COMPREPLY=( $( compgen -W "${opts}" -- "${curr}" ) )
          return 0
        }

        complete -F #{FUNCTION_NAME} #{THIS_FILE}
      BASH
    )
    exit(0)
  end
end.parse!

if options.clear && (options.from || options.to)
  $stderr.puts('Cannot use --clear with --from or --to')
  exit(1)
end

if !options.from and options.to
  $stderr.puts('Cannot use --to without --from')
  exit(1)
end

if !options.to && options.from
  $stderr.puts('Cannot use --from without --to')
  exit(1)
end

if options.to && options.from
  if !all.key?(options.from)
    $stderr.puts("Option --from key #{options.from} does not exist")
    exit(1)
  end

  from = all[options.from]

  if options.from_variant
    from = from.sub(/_\d+/, "_#{options.to_variant}")
  end

  to = nil
  if options.raw
    if options.to_variant
      $stderr.puts("Option --to-variant not allowed with --raw flag")
      exit(1)
    end

    to = options.to
  else
    if !all.key?(options.to)
      $stderr.puts("Option --to key #{options.to} does not exist")
      exit(1)
    end

    to = "#{all[options.to]}.bch"

    if options.to_variant
      to = to.sub(/_\d+/, "_#{options.to_variant}")
    end
  end

  source = "#{options.source_dir}/#{to}"
  target = "#{options.target_dir}/#{from}.bch"

  FileUtils.mkdir_p("#{options.target_dir}/#{File.dirname(from)}")
  FileUtils.copy_file(source, target)
end

if options.clear
  if !all.key?(options.clear)
    $stderr.puts("Option --clear key #{options.clear} does not exist")
    exit(1)
  end

  clear = all[options.clear]
  clear_file = "#{options.target_dir}/#{clear}.bch"

  FileUtils.remove_file(clear_file)
end
