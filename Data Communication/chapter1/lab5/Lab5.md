# Lab 5:HTTP Web Proxy Server

#### 191180048 黄奥成 通信工程

==Attention：此报告是由markdown编辑，导出的pdf会有排版错误，请尽量阅读markdown版本==

### 一、实验原理与准备

在个编程作业中，研发一个的Web代理服务器。当你的代理服务器从一个浏览器收到某对象的HTTP请求，它生成对相同对象的一个新HTTP请求并向初始服务器发送。当该代理从初始服务器接收到具有该对象的HTTP响应时，它生成一个包括该对象的新HTTP响应，并发送给该客户。这个代理将是多线程的，使其在相同时间能够处理多个请求。



![image-20220320103024264](C:\Users\Hacetate\Desktop\数据通信\Lab5\Lab5.assets\image-20220320103024264.png)



由于之前~~科学上网~~时挂代理设置过，因此在做实验前先备份设置，然后设置实验环境



<figure>
<img src="C:\Users\Hacetate\Desktop\数据通信\Lab5\Lab5.assets\image-20220319121804385.png" alt="image-20220319121804385" style="zoom: 80%;" />
<img src="C:\Users\Hacetate\Desktop\数据通信\Lab5\Lab5.assets\image-20220319121743204.png" alt="image-20220319121743204" style="zoom: 80%;" />
</figure>

### 二、实验过程与结果分析

##### 1、输出

首先把实验结果挂出来，接下来结合代码与输出来分析

##### ![image-20220320201854835](C:\Users\Hacetate\Desktop\数据通信\Lab5\Lab5.assets\image-20220320201854835.png)

![image-20220320204207527](C:\Users\Hacetate\Desktop\数据通信\Lab5\Lab5.assets\image-20220320204207527.png)



##### 2、服务端socket连接与接收报文

创建一个服务端socket用于接收报文

```python
tcpSerSock = socket(AF_INET, SOCK_STREAM)
# Fill in start.
serverPort = 7890
tcpSerSock.bind(('',serverPort))
tcpSerSock.listen(1)
# Fill in end.
while 1:
	# Strat receiving data from the client
	print('Ready to serve...')
	tcpCliSock, addr = tcpSerSock.accept()
	print('Received a connection from:', addr)
	message = tcpCliSock.recv(4096)
	print(message)

```

![image-20220320202350576](C:\Users\Hacetate\Desktop\数据通信\Lab5\Lab5.assets\image-20220320202350576.png)



##### 3、将报文中的地址提取并处理

由于windows文件格式中不能出现=='/'==符号，因此将其替换为=='__'==。也即`filetouse`

```python
    # Extract the filename from the given message
    print(message.split()[1])
    filename = message.split()[1].decode().partition("/")[2]
    filetouse = message.split()[1].decode().partition("//")[2].replace('/', '_')
    print('filename = ' + filename)
    fileExist = "false"
    print('filetouse = ' + filetouse)

```

![image-20220321084922183](C:\Users\Hacetate\Desktop\数据通信\Lab5\Lab5.assets\image-20220321084922183.png)

##### 4、尝试打开缓存

若open成功，则具有此网页的缓存。若失败，跳转到`IOError`

```python
try:# Check wether the file exist in the cache
		f = open(filetouse[0:], "r") 
		outputdata = f.readlines() 
		print("fileexist = true")
		fileExist = "true"
		# ProxyServer finds a cache hit and generates a response message
		#tcpCliSock.send("HTTP/1.0 200 OK\r\n") 
		#tcpCliSock.send("Content-Type:text/html\r\n")
		# Fill in start
		for i in range(0 , len(outputdata)):
    			tcpCliSock.send(outputdata[i].encode())
		tcpCliSock.send('\r\n'.encode())
		tcpCliSock.close()

		# Fill in end.
		print('Read from cache') 
		# Error handling for file not found in cache
```

![image-20220320204207527](C:\Users\Hacetate\Desktop\数据通信\Lab5\Lab5.assets\image-20220320204207527.png)



##### 5、缓存网页

创建一个`clientsocket`用于发送报文（转发报文）。连接80端口并且将之前接收的请求报文发送给"真正"的服务端。然后接收其返回的报文`response`。将其写入以`filetouse` 为文件名的文件中

```python
except IOError:
		if fileExist == "false": 
			# Create a socket on the proxyserver
			tcpCacheSock = socket(AF_INET, SOCK_STREAM)
			hostn = filename.replace("www.","",1) 
			print('hostn = ' + hostn)
            
			try:
				# Connect to the socket to port 80
				tcpCacheSockPort = 80
				serverName = hostn.split("/")[1]
				print((serverName, tcpCacheSockPort))
				tcpCacheSock.connect((serverName, tcpCacheSockPort))
				print('Socket connected to port 80 of the host')
				tcpCacheSock.send(message)
				print('Send the message sucessfully!')
				#获取服务器返回内容
				response = b''
				rec = tcpCacheSock.recv(4096)
				while rec:
					response += rec
					rec = tcpCacheSock.recv(4096)

				print('response = ' + response.decode())
				tmpFile = open("./" + filetouse,"w")
				print('open the file!')
				tmpFile.writelines(response.decode().replace('\r\n','\n'))
				tmpFile.close()
				#sys.exit()

			except:
				print("Illegal request")
		else:
			# HTTP response message for file not found
			print('ERROR!!')
			# Close the client and the server sockets 
	tcpCliSock.close() 
tcpSerSock.close()

```



### 三、option experience

##### 1、 404 NOT FOUND

若网页不存在，则返回的报文中使用`spilt`分离后的字节流为`404` 。

![image-20220321091522085](C:\Users\Hacetate\Desktop\数据通信\Lab5\Lab5.assets\image-20220321091522085.png)

```python

response = b''
rec = tcpCacheSock.recv(4096)
while rec:
	response += rec
	rec = tcpCacheSock.recv(4096)

if response.split()[0] == b'404':
	print('404')
	tcpCliSock.send("HTTP/1.1 404 Not Found\r\n\r\n".encode())
	tcpCliSock.close()
	continue

tmpFile = open("./" + filetouse,"w")
tmpFile.writelines(response.decode().replace('\r\n','\n'))

```

![image-20220320233420898](C:\Users\Hacetate\Desktop\数据通信\Lab5\Lab5.assets\image-20220320233420898.png)



##### 2、POST方法

```python
if(message.split()[0] == 'GET'):
                	fileobj.write("GET ".encode() + ''.join(filename.partition('/')[1:]).encode() + " HTTP/1.0\r\nHost: 										".encode() + serverName.encode() + "\r\n\r\n".encode())
     
else: #POST
                    fileobj.write("POST ".encode() + ''.join(filename.partition('/')[1:]).encode() + " HTTP/1.0\r\nHost: 									".encode() + serverName.encode() + "\r\n\r\n".encode())
                    fileobj.write(message.split("\r\n\r\n")[1].encode())
```



### 四、Something interesting	

##### 1、不使用本地连接

由于我做实验的机器为台式机（后称PC）且其始终连接校园网（网线）为内网，因而很容易的在校内各处访问我的PC。这里使用我的笔记本尝试访问！

保持PC的代理打开

很不幸的是，我的笔记本与PC发送的报文的编码方式不同

![image-20220321095816917](C:\Users\Hacetate\AppData\Roaming\Typora\typora-user-images\image-20220321095816917.png)

由于时间原因（ddl貌似已经到了），下次实验报告会补全这部分内容

##### 2、转发至公网

使用sakura frp 转发至公网。

![image-20220321101237876](C:\Users\Hacetate\AppData\Roaming\Typora\typora-user-images\image-20220321101237876.png)











1. **TypeError**: a bytes-like object is required, not 'str'  : https://blog.csdn.net/u011675334/article/details/108768482

   

