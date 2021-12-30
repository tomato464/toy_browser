We make a subset of ping command.

The basic is : 
make socket (socket(domain, type, protocol)) (include<sys/socket.h>)
send packet (sendto(socket_fd, buf, buf_len, flags, dst_addr, dst_addr_len)) include<sys/socket.h>
recv packet (recvmsg(sockfd, msg, flags)) (include<socket.h>)
