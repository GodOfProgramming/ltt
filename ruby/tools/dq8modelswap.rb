#!/usr/bin/env ruby

require 'optparse'
require 'ostruct'
require 'fileutils'

def format_hash(prefix, hash)
  hash.map { |k, v| [ "#{prefix}/#{k}", "#{prefix}/#{v}" ] }.to_h
end

players = format_hash('player', {
  hero: 'c001_1',
  yangus: 'c002_1',
  jessica: 'c004_1',
  angelo: 'c003_1',
  trode: 'c005a',
  medea: 'c006a',
  munchi: 'c007a',
  red: 'c011_1',
  morrie: 'c012_1',
})

npcs = format_hash('npc', {
  angelo_child: 'dp005',
  bangerz: 'bp001',
  bard: 'p010a',
  bard_thorn: 'p010ap',
  bird_landing_icon: 'taka_f_cursor',
  blonde_bowlcut_guy: 'p013a',
  blonde_girl: 'p069a',
  casino_guy: 'gp001',
  eagle: 'a003a',
  elvin_girl: 'op001',
  emma: 'dp003',
  ishmahri: 'dp004',
  kalderasha: 'ap001',
  king_pavan: 'dp001',
  lizard: 'a002a',
  man_in_green_and_purple: 'lp001',
  man_in_purple: 'p022a',
  marchello: 'cp001',
  marchello_child: 'dp006',
  market_girl: 'p046a',
  medea_human: 'fp002',
  merchant: 'dqtoruneko',
  monsters_fighting: 'ep004',
  morrie: 'ep003',
  munchi: 'a001a',
  muscular_criminal: 'p056a',
  painting: 's001a',
  priestess: 'ip001',
  prince_charmles_of_argonia: 'jp001',
  queen_sasha: 'dp002',
  red: 'ep001',
  templar_guard: 'p040a',
  thief: 'ep002',
  trode_human: 'fp001',
  valentina: 'ap002',
})

monsters = format_hash('monster', {
  archdemon_regular: 'm000',
  archdemon_yellow: 'm001',
  bullfinch: 'm002',
})

all = players.merge(npcs).merge(monsters)

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
