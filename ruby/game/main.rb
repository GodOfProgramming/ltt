require 'ruby2d'

class Square
  def move(x: 0, y: 0)
    self.x = self.x + x
    self.y = self.y + y
  end
end

class Keyboard
  def initialize
    @keys = Hash.new
  end

  def keys
    @keys
  end

  def set(keyval)
    keyval.each do |key, val|
      @keys[key] = val
    end
  end

  def get(key)
    @keys[key]
  end

  def [] key
    self.get(key)
  end
end

# Set the window size
set width: 1920, height: 1080

# Create a new shape
s = Square.new

# Give it some color
s.color = 'red'

on :mouse_down do |e|
  puts "Clicked #{e.button}"
end

keys = Keyboard.new

movethread = Thread.new {
  loop do
    keys.keys.each do |key, val|
      if val
        case key
        when :a
          s.move x: -1
        when :s
          s.move y: 1
        when :w
          s.move y: -1
        when :d
          s.move x: 1
        end
      end
    end
    sleep(1.0 / 60.0)
  end
 }

on :key_down do |e|
  case e.key
  when 'a'
    keys.set a: true
  when 's'
    keys.set s: true
  when 'w'
    keys.set w: true
  when 'd'
    keys.set d: true
  end
end

on :key_up do |e|
  case e.key
  when 'a'
    keys.set a: false
  when 's'
    keys.set s: false
  when 'w'
    keys.set w: false
  when 'd'
    keys.set d: false
  end
end

# Show the window
show
Thread.kill(movethread)
