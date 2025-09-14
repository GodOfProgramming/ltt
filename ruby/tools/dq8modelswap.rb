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
  bartender: 'p021a',
  bird_landing_icon: 'taka_f_cursor',
  blacksmith: 'p012a',
  blonde_bowlcut_guy: 'p013a',
  boy: 'p018a',
  boy_monk: 'p016a',
  boy_teenager: 'p019a',
  boy_tux_blue_hair: 'x002',
  boy_tux_brown_hair: 'x003',
  eagle: 'a003a',
  emma: 'dp003',
  girl_blonde: 'p069a',
  girl_blonde_pigtails: 'p051a',
  girl_elvin: 'op001',
  girl_maid: 'p020a',
  girl_market: 'p046a',
  girl_scholar_mourning: 'p045a',
  guard_red: 'p014a',
  guard_templar: 'p040a',
  guy_casino: 'gp001',
  hero_child: 'x001',
  invisible: 'rg01',
  ishmahri: 'dp004',
  jessica_wedding: 'x006',
  kalderasha: 'ap001',
  king_pavan: 'dp001',
  lady_old: 'p011a',
  lady_old_happy: 'p027a',
  lizard: 'a002a',
  lord_high_priest: 'pp002',
  man_beggar: 'p050a',
  man_bishop: 'p038a',
  man_chef: 'p053a',
  man_dealer: 'p036a',
  man_farmer: 'p049a',
  man_guard: 'p048a',
  man_jailer_guard: 'p056a',
  man_merc_elf: 'p057a',
  man_merc_pink: 'p055a',
  man_merchant_small: 'p043a',
  man_monk: 'p029a',
  man_monk_old: 'p030a',
  man_noble: 'p028a',
  man_old: 'p033a',
  man_pirate: 'p025a',
  man_purple: 'p022a',
  man_purple_and_green: 'lp001',
  man_scholar: 'p031a',
  man_scholar_mourning: 'p044a',
  man_templar: 'p032a',
  man_templar_big_lips: 'p041a',
  man_templar_older1: 'p075a',
  man_templar_older2: 'p040a',
  man_templar_older3: 'p042a',
  man_warrior: 'p035a',
  marcello: 'cp001',
  marcello_child: 'dp006',
  marcello_lord_high_priest: 'pp001',
  marcello_possessed: 'pp001b',
  medea_child: 'x004',
  medea_human: 'fp002',
  medea_wedding_1: 'rp001',
  medea_wedding_2: 'x005',
  merchant: 'dqtoruneko',
  monsters_fighting: 'ep004',
  morrie: 'ep003',
  munchi: 'a001a',
  muscular_criminal: 'p056a',
  nun: 'p039a',
  painting: 's001a',
  priest: 'p034a',
  priestess: 'ip001',
  prince_charmles: 'jp001',
  queen_sasha: 'dp002',
  red: 'ep001',
  sabercat_black: 'x008',
  sabercat_white: 'x007',
  salesman_herb: 'p023a',
  swordsman_blue: 'p017a',
  swordsman_green: 'p015a',
  thief: 'ep002',
  thief_man: 'p026a',
  trode_human: 'fp001',
  valentina: 'ap002',
  woman_bunny_black_blue: 'p024a_iro2',
  woman_bunny_black_green: 'p024a',
  woman_bunny_black_pink: 'p024a_iro',
  woman_bunny_black_yellow: 'p024a_iro3',
  woman_fortune_teller: 'p037a',
  woman_housewife: 'p052a',
  woman_merc: 'p047a',
  woman_merchant: 'p046a',
  woman_noble: 'p054a',
})

monsters = format_hash('monster', {
  archdemon_regular: 'm000',
  archdemon_yellow: 'm001',
  bullfinch: 'm002',
  purple_eye: 'm123',
  killing_machine: 'm100',
  stone_golem: 'm030',
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
