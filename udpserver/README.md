In this repository, I make udp server.(somuthing like $ nc -l -u 0.0.0.0 12345)

How it works:

create socket
make a relation between socket and address, port

while(1)
{
	receive message
	print message
}
