# server.rb port_no
# server.rb 2000

require 'socket'               # Sockets are in standard library

server_port = ARGV[0].to_i

if !File.exist?('ctg.marshal') then
  ctg = {}
  gtc = {}
  File.open("ctg.marshal", "w+"){|to_file| Marshal.dump(ctg, to_file)}
  File.open("gtc.marshal", "w+"){|to_file| Marshal.dump(gtc, to_file)}
end

# open socket
socket_handle = TCPServer.open(server_port)

loop {  # Servers run forever
  # adding thread support
  Thread.start(socket_handle.accept) do |client_handle|

    # read hash from files
    ctg = File.open("ctg.marshal", "r"){|from_file| Marshal.load(from_file)}
    gtc = File.open("gtc.marshal", "r"){|from_file| Marshal.load(from_file)}

    # first thing expected from client is its id
    client_id =  client_handle.gets.to_i
    puts "Hi #{client_id} !"
    client_handle.puts "Hi #{client_id} !"

    # give client_to_group(s) mapping
    if ctg.has_key? (client_id) then
      client_handle.puts "You are part of #{ctg[client_id]}"
    else
      client_handle.puts "You have not joined any group"
    end

    # receive group_id/x and update mapping
    group_id = client_handle.gets
    if /x/.match(group_id) then
      puts "client don't want to join !"
    else
        # updating ctg hash_map
        if ctg[client_id].nil? then
          ctg[client_id] = []
        end
        temp_arr  = []
        temp_arr = ctg[client_id]
        temp_arr.push(group_id)
        ctg[client_id] = temp_arr

        # updating gtc hash-map
        if gtc[group_id].nil? then
          gtc[group_id] = []
        end
        temp_arr = []
        temp_arr = gtc[group_id]
        temp_arr.push(client_id)
        gtc[group_id] = temp_arr
    end

    #reading client message for group id
    msg = client_handle.gets
    puts msg

    # writing msg to group-files
    group_arr = []
    group_arr = ctg[client_id]
    group_arr.each { |group_id_filename|
      begin
        file = File.open("#{group_id_filename.to_i}", "a")
        file.write("client #{client_id} : #{msg}")
      rescue IOError => e
      #some error occur, dir not writable etc.
      ensure
        file.close unless file.nil?
      end
    }

    File.open("ctg.marshal", "w+"){|to_file| Marshal.dump(ctg, to_file)}
    File.open("gtc.marshal", "w+"){|to_file| Marshal.dump(gtc, to_file)}

    # closing connection to client
    client_handle.close
  end

}
