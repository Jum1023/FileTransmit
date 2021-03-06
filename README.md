# Network
### purpose for this repositories
1. Basic code used for learning network with POSIX functions
2. Test for the speed of each implementation

### protocol
1. TCP

* Linux TCP server and client demo

* linux查看socket连接

```shell
netstat -antp
```

* mac查看socket连接

```shell
lsof -p PID | grep TCP
```

2. UDP

	Linux UDP server and client demo

3. HTTP

	HTTP client and server demo

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

2. FIN-WAIT-1

	client先异常断开没有发送FIN,server主动关闭进入FIN-WAIT-1 \
	无法收到FIN,会一直处于该状态,该状态系统有time_out,尚未查证具体时间
```shell
# record what tcp_max_orphans's current value
original_value=$(cat /proc/sys/net/ipv4/tcp_max_orphans)

#set the tcp_max_orphans to 0 temporarily
echo 0 > /proc/sys/net/ipv4/tcp_max_orphans

# watch /var/log/messages
# it will split out "kernel: TCP: too many of orphaned sockets"
# it won't take long for the connections to be killed

# restore the value of tcp_max_orphans whatever it was before.
echo $original_value > /proc/sys/net/ipv4/tcp_max_orphans
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

### reactor & proactor

	两种高效的IO事件处理模型

### 单台服务器百万并发

1. server系统限制
* 单个进程最多打开的文件数
```shell
ulimit -a # 查看全部
ulimit -n # 关注open files

vim /etc/security/limits.conf
修改fs.file_max
```
* 单个进程的maxfd
```shell
sysctl -a | grep file_max
# 修改文件
/etc/sysctl.conf
net.ipv4.tcp_men = 262144 524288 786432
net.ipv4.tcp_wmen = 1024 1024 2048
net.ipv4.tcp_rmen = 1024 1024 2048
fs.file_max =1048576 #max fd

sysctl -p #生效
```
2. 单个客户端最多连接数限制

	server端开发不用关注，单台客户端机器测试用
```shell
# 修改文件
/etc/sysctl.conf
net.nf_conntrack_max = 1048576
net.netfilter.nf_conntrack_tcp_timeout_established = 1200

sysctl -p #生效
```

### 单机服务器IO模型
1. 单线程同步 NTP
2. 多线程同步 Natty
3. 纯异步 redis,HAProxy
4. 半同步半异步 Natty
5. 多进程同步 fastcgi
6. 多线程异步 memcached
7. 多进程异步 nginx
8. 每请求每进程(线程) Apache/CGI
9. 微进程架构 erlang/go/lua
10. 协程架构 libco

### opensource network framework

> boost.asio
> libevent
> https://github.com/Tencent/mars