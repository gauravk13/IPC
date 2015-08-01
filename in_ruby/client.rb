# client.rb server_hostname port_no client_id
# client.rb localhost 2000 4

require 'socket'      # Sockets are in standard library

server_hostname = ARGV[0]
server_port = ARGV[1].to_i
client_id = ARGV[2].to_i

# open socket
socket_handle = TCPSocket.open(server_hostname, server_port)

# send client id and expects a Hi <client-id> message in return
socket_handle.puts "#{client_id}"
puts socket_handle.gets

# group mapping info
puts socket_handle.gets

puts "enter group_id to join , else press 'x' : "
inp = STDIN.gets.chomp

# send group_id /x to server
socket_handle.puts "#{inp}"

#send message
puts "enter your message :"
inp = STDIN.gets()
socket_handle.puts "#{inp}"

# Close the socket when done
socket_handle.close
#=end
