# Network
### Basic Features

1. File transfer,message transfer,based on both TCP and UDP protocal
2. Display TCP and UDP Datagram content both string and hex format
3. Send datagram from textedit and files with the control of transmission rate
4. Complete send and receive datagram from multicast
5. Record and playback datagram
6. Log record with hierarchy

### Advanced protocol

1. http
2. rtsp
3. vpn
4. shadowsocks

### Module Design

1. Network cross platforms(boost)
2. Memory pool with high concurrency(eg:semphore for both producer and consumer)
3. thread model cross platforms(boost,C++14)

### Technical Details

1. socket option with block and non block settings
2. socket option with join and drop the group
3. json,xml config file settings
4. breaking point supervention of files
5. memory map with large files
6. one physical address(mac) with many IPs(the same mac in the same local Local Area Networks)
7. multi files transfer

### Other Requirement

1. All data structure should be armed with CRUD operations(create,retrieve,update,delete)
