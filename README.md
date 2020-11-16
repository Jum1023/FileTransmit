# Network
### purpose for this repositories
1. Basic code used for learning network with POSIX functions
2. Test for the speed of each implementation

### protocol
1. TCP

* Linux TCP server and client demo

* linux查看socket连接

```shell
netstat -atp
```

* mac查看socket连接

```shell
lsof -p PID | grep TCP
```

2. UDP

Linux UDP server and client demo

3. HTTP

### TCP State

* LISTEN represents waiting for a connection request from any remote TCP and port.
* SYN-SENT represents waiting for a matching connection request after having sent a connection request.
* SYN-RECEIVED represents waiting for a confirming connection request acknowledgment after having both received and sent a connection request.
* ESTABLISHED represents an open connection, data received can be delivered to the user. The normal state for the data transfer phase of the connection.
* FIN-WAIT-1 represents waiting for a connection termination request from the remote TCP, or an acknowledgment of the connection termination request previously sent.
* FIN-WAIT-2 represents waiting for a connection termination request from the remote TCP.
* CLOSE-WAIT represents waiting for a connection termination request from the local user.
* CLOSING represents waiting for a connection termination request acknowledgment from the remote
TCP.
* LAST-ACK represents waiting for an acknowledgment of the connection termination request previously sent to the remote TCP (which includes an acknowledgment of its connection termination
request).
* TIME-WAIT represents waiting for enough time to pass to be sure the remote TCP received the
acknowledgment of its connection termination request.
* CLOSED represents no connection state at all.

#### establish

```C++
/*
state			local			remote			state
CLOSED			|					|			CLOSED
				|					|
				|	  SYN	seq=n	|			listen
----------------|==================>|----------------------
				|					|
				|	ACK	ack_seq=n+1	|
				|	SYN seq=m		|
SYN-SENT		|<==================|			SYN-RECEIVED
				|					|
				|	ACK	ack_seq=m+1	|
----------------|==================>|----------------------
				|					|
ESTABLISHED		|					|			ESTABLISHED
*/
```

#### close

```C++
/*
state			local			remote			state
ESTABLISHED		|					|			ESTABLISHED
				|					|
				|	  FIN	seq=n	|
----------------|==================>|----------------------
FIN-WAIT-1		|					|
				|	ACK	ack_seq=n+1	|
----------------|<==================|			CLOSE-WAIT
				|	  FIN   seq=n	|
				|<==================|----------------------
FIN-WAIT-2		|					|
				|					|			LAST-ACK
				|	ACK	ack_seq=n+1	|
----------------|==================>|----------------------
				|					|
TIME-WAIT		|					|
				|					|
----------------|					|			CLOSED
				|					|
CLOSED			|					|
*/
```

#### 常见名词
* 超时重传
* 慢启动
* 拥塞控制
* 滑动窗口
* 三次握手
* 四次挥手

### epoll

1. 百万并发

linux百万并发，要修改单个线程可以打开的文件数
```shell
ulimit -n 1000000
```

### reactor & proactor

两种高效的IO事件处理模型