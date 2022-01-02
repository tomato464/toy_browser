In this repository, I make basic http server.(do just 1-1 connection.)

how it works:

(parameter : tcp , port from command line)

make socket
loop
{
recv
sendto
}


*tcp{
socket (SOCK_STREAM)
bind ( select packet, bind between dst_ip_address and socket)
lesten
accept (<= connection based socket-types SOCK_STREAM, SOCK_SEQPACKET)
(accept may need because, connection based protocol do extra-things like 3-way hand shake.
those things require to clearfy which side is client and, which is server. ????)
(file_discriptor accept())
read

after make responce

sendto

close accepted_socket (original socket is alive)
}
*udp{
socket (SOCK_DGRAM)
bind
recvfrom

after make response

sendto
}
