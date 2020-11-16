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

1. TIME-WAIT

当最后一个ack丢失时，remote端启动定时重传fin,如果没有TIME-WAIT,会一直发出无效包

默认值120s，在linux下设置方式
```shell
/etc/sysctl.conf
# 表示开启SYN Cookies。当出现SYN等待队列溢出时，启用Cookie来处理，可防范少量的SYN攻击。该参数默认为0，表示关闭
net.ipv4.tcp_syncookies＝1
# 表示开启重用，即允许将TIME-WAIT套接字重新用于新的TCP连接。该参数默认为0，表示关闭
net.ipv4.tcp_tw_reuse＝1
# 表示开启TCP连接中TIME-WAIT套接字的快速回收，该参数默认为0，表示关闭
net.ipv4.tcp_tw_recycle＝1
# 表示如果套接字由本端要求关闭，那么这个参数将决定它保持在FIN-WAIT-2状态的时间
net.ipv4.tcp_fin_timeout＝30
# 表示当Keepalived启用时，TCP发送Keepalived消息的频度改为20分钟，默认值是2小时
net.ipv4.tcp_keepalive_time＝1200
# 表示系统同时保持TIME_WAIT套接字的最大数量，如果超过这个数字，TIME_WAIT套接字将立刻被清除并打印警告信息，默认值为180 000，此处改为5000。对于Apache、Nginx等服务器，前面介绍的几个参数已经可以很好地减少TIME_WAIT套接字的数量，但是对于Squid来说，效果却不大，有了此参数就可以控制TIME_WAIT套接字的最大数量，避免Squid服务器被大量的TIME_WAIT套接字拖死
net.ipv4.tcp_max_tw_buckets＝5000

sysctl -p /etc/sysctl.conf
```

#### 常见名词
* 慢启动

门限值以下是慢启动，以上是拥塞控制

* 拥塞控制

rtt(round trip time) rtt = 0.9*pre_rtt + cur_rtt
rtt增大window减小，rtt减小window增大

* 滑动窗口 [ack,ack+window]
* 超时重传
* 延迟确认
* 半连接/全连接(syn队列/accept队列)
* 全双工/半双工
* 三次握手
* 四次挥手

#### TCP vs UDP

1. UDP实时性高于TCP
2. UDP传输效率高于TCP

### epoll

1. 百万并发

linux百万并发，要修改单个线程可以打开的文件数
```shell
ulimit -n 1000000
```

### reactor & proactor

两种高效的IO事件处理模型