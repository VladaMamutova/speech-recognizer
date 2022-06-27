mode = 0
theme = ''
title = ''
count = 1
line_number = 0
new_file = nil

file = File.new('data.txt')
file_info = File.new('file_info.txt', 'w')
file.each do |line|
  line_number = line_number.next

  if line =~ /=====/
    if mode == 3
      if new_file
        new_file.close
        new_file = nil

        count = count.next
        mode = 0
      end
    elsif mode != 0
      raise "Error in #{line_number}, found '=====' in mode != 0 && mode != 3"
    end
    next
  end

  case mode
  when 0
    theme = line.rstrip
  when 1
    title = line.rstrip
  when 2
    raise "Error in #{line_number}, expected '-----'" unless line =~ /-----/
  when 3
    file_info.puts "#{count}. #{theme}. #{title}.txt" unless new_file

    new_file ||= File.new("#{count}. #{title}", 'w')
    new_file.puts(line)
  else
    "Unknown mode in #{line_number}"
  end

  mode = mode.next if [0, 1, 2].include? mode
end

file.close
file_info.close
