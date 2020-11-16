# Network
### purpose for this repositories
1. Basic code used for learning network with POSIX functions
2. Test for the speed of each implementation

### protocol
1. TCP

* Linux TCP server and client demo

* linux查看socket连接

```shell
netstat -tp
```

* mac查看socket连接

```shell
lsof -p PID | grep TCP
```

2. UDP

Linux UDP server and client demo

3. HTTP

### epoll

1. 百万并发

linux百万并发，要修改单个线程可以打开的文件数
```shell
ulimit -n 1000000
```

### reactor & proactor

两种高效的IO事件处理模型