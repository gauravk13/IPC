loop {

puts "\n1. display group to client mapping"
puts "2. display client to group mappings"
puts "3. display messages of group id :"
puts "x to exit\n"

inp = STDIN.gets.chomp

  if inp.to_i == 1 then

    # group to client
    gtc = File.open("gtc.marshal", "r"){|from_file| Marshal.load(from_file)}
    puts "group_id :=> client_id list"
    gtc.each do |key,value|
      puts "#{key.to_s} :=> #{value}"
    end
    puts "\n"
  elsif inp.to_i == 2 then

    # client to group
    ctg = File.open("ctg.marshal", "r"){|from_file| Marshal.load(from_file)}
    puts "client_id :=> group_id list"
    ctg.each do |key,value|
      puts "#{key.to_s} :=> #{value}"
    end

  elsif inp.to_i == 3 then

    puts "enter group id :"
    inp = STDIN.gets.chomp
    puts "messages of group #{inp}"
    File.open("#{inp}", "r") do |f|
      f.each_line do |line|
        puts line
      end
    end

  elsif inp == "x" then
    exit
  else
    puts "\ninvalid input ! try again "
  end

}