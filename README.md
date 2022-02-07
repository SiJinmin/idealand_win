# 理想乐园元宇宙（Idealand）v0.8软件发布说明书

理想乐园元宇宙（Idealand）的目标是成为一个有利于大家的日常生活的有趣的元宇宙平台，目前还处于练习使用C语言编程的初级阶段，提供的功能十分简单脆弱，相信未来经过我们的持续努力，它一定会走向强大稳健。

目前提供了批量下载文件的功能，可通过配置文件指定下载文件的起始号和结束号，支持断线重连和智能的断点续传。此外它有如下特点：
1. 绿色软件，仅有1个大小均为1.3M的exe文件(idealand.exe)，和一个配置文件(idealand.conf)，没有其它任何文件。
2. 自由免费开源软件，所有源代码将通过MIT许可发布到github(https://github.com/SiJinmin/idealand_win)
3. 编程语言为C，十分高效。
4. 代码结构严谨，开发者对代码的质量要求很高，这包括抗攻击性、架构的合理性等。


# 如何使用

目前仅能在Windows上运行，接下来会兼容Linux，编译好的可执行文件在release文件夹中（release/idealand.exe）


## 提供文件下载服务

1. 提供文件下载服务的服务器暂时必须有公网IP（接下来会改成p2p，支持局域网，不再需要公网IP）。
2. 服务器必须在防火墙中开放27015端口，允许客户端通过TCP连接该端口，启动时允许本程序监听该端口。
3. 通过Windows终端命令行（cmd.exe）运行下面的命令，其中idealand是 release/idealand.exe 可执行程序，s表示以提供文件下载服务的方式启动。
    ```
    idealand s
    ```
4. 运行上面的命令后会自动在idealand.exe所在的目录创建一个叫IdealandData的文件夹，这里是放置下载文件的地方。
5. 在IdealandData文件夹里创建的文件夹叫集合，可创建任意多个集合。
6. 每个集合文件夹里面可放置任意多个文件，这些文件必须以四个数字开头，比如0001.mp4, 0003.瑜伽师地论.mp4 等，用户下载时指定的起始编号和结束编号就是指文件名开头的4个数字。


## 下载文件

1. 配置文件（release/idealand.conf）里的设置指定了想要下载的文件，包括：服务器域名或者IP， 下载集合名， 起始文件号， 结束文件号。
   示例idealand.conf文件中的设置是从域名为 idealand.xyz 的服务上下载“妙境法师讲解《瑜伽师地论》MP4视频”这个集合里的从22到40这19个文件。
   需要注意的是：idealand.conf 文件是utf8(65001)编码，修改这个文件以后保存时请注意不要更改它的编码。
2. 通过Windows终端命令行（cmd.exe）运行下面的命令，其中idealand是 release/idealand.exe 可执行程序，d表示以下载文件的方式启动。
    ```
    idealand d
    ```
3. 运行上面的命令后会自动在idealand.exe所在的目录创建一个叫IdealandData的文件夹，这里是放置下载文件的地方，然后会开始下载配置文件中指定的文件。
4. 在下载的过程中可随时通过ctr+c终止下载。
5. 因为idealand.exe有智能的断点续传功能，下次运行相同的命令时，会自动扫描现有的已下载的文件的大小并进行断点续传。


## 全能模式

上面两种启动方式分别使用了 s 和 d 作为启动参数，如果想在提供文件下载服务的同时也下载文件，可使用下面的命令，也就是不提供任何启动参数。
```
idealand
```





# 如何参与开发
```
git clone https://github.com/SiJinmin/idealand_win
```
请用visual studio community 2022打开Win.sln文件，项目文件在C文件夹

下载文件的功能需在 idealand.exe 所在的目录中放置配置文件 release/idealand.conf


# 理想乐园元宇宙（Idealand）软件编码规范说明

## A 函数返回值规范

  默认的函数返回值规则为：
  1. 返回值类型为int或者INT64
  2. 负数表示是错误代码
      -1 means system error 
      -2 means socket connection or transfer error
      -3 means cannot open specified file path
      -4 means not found
  3. 整数和零表示是成功和有效的返回值

  符合默认规则的函数不必对返回值进行说明，而不符合此规则的返回值则应在注释中说明。
  

## B 资源释放说明  

  如果函数里面生成了资源且不能立刻释放，应注明caller释放资源的方法和条件


## C 命名规则

  1. 所有变量均以idealand开头，以作为名称空间。
  2. 除了idealand作为总名称空间，还有子名称空间，子名称空间可以有多层，比如idealand_socket，idealand_string，idealand_socket_check
  3. 常规函数以idealand_subnamespaces_function_name的方式命名，比如 idealand_socket_send
  4. #define常量、const常量、typedef的类型以IdealandTheName的方式命名，比如IdealandBufferSize











