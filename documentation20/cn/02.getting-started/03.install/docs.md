
# TDengine 安装包的安装和卸载

TDengine 开源版本提供 deb 和 rpm 格式安装包，用户可以根据自己的运行环境选择合适的安装包。其中 deb 支持 Debian/Ubuntu 等系统，rpm 支持 CentOS/RHEL/SUSE 等系统。同时我们也为企业用户提供 tar.gz 格式安装包。

## deb 包的安装和卸载

### 安装 deb

1、从官网下载获得deb安装包，比如TDengine-server-2.0.0.0-Linux-x64.deb；
2、进入到TDengine-server-2.0.0.0-Linux-x64.deb安装包所在目录，执行如下的安装命令：

```
$ sudo dpkg -i TDengine-server-2.4.0.7-Linux-x64.deb
(Reading database ... 137504 files and directories currently installed.)
Preparing to unpack TDengine-server-2.4.0.7-Linux-x64.deb ...
TDengine is removed successfully!
Unpacking tdengine (2.4.0.7) over (2.4.0.7) ...
Setting up tdengine (2.4.0.7) ...
Start to install TDengine...

System hostname is: shuduo-1804

Enter FQDN:port (like h1.taosdata.com:6030) of an existing TDengine cluster node to join
OR leave it blank to build one:

Enter your email address for priority support or enter empty to skip:
Created symlink /etc/systemd/system/multi-user.target.wants/taosd.service → /etc/systemd/system/taosd.service.

To configure TDengine : edit /etc/taos/taos.cfg
To start TDengine     : sudo systemctl start taosd
To access TDengine    : taos -h shuduo-1804 to login into TDengine server


TDengine is installed successfully!
```

注：当安装第一个节点时，出现 Enter FQDN：提示的时候，不需要输入任何内容。只有当安装第二个或以后更多的节点时，才需要输入已有集群中任何一个可用节点的 FQDN，支持该新节点加入集群。当然也可以不输入，而是在新节点启动前，配置到新节点的配置文件中。

后续两种安装包也是同样的操作。

### 卸载 deb

卸载命令如下:

```
$ sudo dpkg -r tdengine
(Reading database ... 137504 files and directories currently installed.)
Removing tdengine (2.4.0.7) ...
TDengine is removed successfully!
```

## rpm包的安装和卸载

### 安装 rpm

1、从官网下载获得rpm安装包，比如TDengine-server-2.0.0.0-Linux-x64.rpm；
2、进入到TDengine-server-2.0.0.0-Linux-x64.rpm安装包所在目录，执行如下的安装命令：

```
$ sudo rpm -ivh TDengine-server-2.4.0.7-Linux-x64.rpm
Preparing...                          ################################# [100%]
Updating / installing...
   1:tdengine-2.4.0.7-3               ################################# [100%]
Start to install TDengine...

System hostname is: centos7

Enter FQDN:port (like h1.taosdata.com:6030) of an existing TDengine cluster node to join
OR leave it blank to build one:

Enter your email address for priority support or enter empty to skip:

Created symlink from /etc/systemd/system/multi-user.target.wants/taosd.service to /etc/systemd/system/taosd.service.

To configure TDengine : edit /etc/taos/taos.cfg
To start TDengine     : sudo systemctl start taosd
To access TDengine    : taos -h centos7 to login into TDengine server


TDengine is installed successfully!
```

### 卸载 rpm

卸载命令如下:

```
$ sudo rpm -e tdengine
TDengine is removed successfully!
```

## tar.gz 格式安装包的安装和卸载

### 安装 tar.gz 安装包

1、从官网下载获得tar.gz安装包，比如TDengine-server-2.0.0.0-Linux-x64.tar.gz；
2、进入到TDengine-server-2.0.0.0-Linux-x64.tar.gz安装包所在目录，先解压文件后，进入子目录，执行其中的install.sh安装脚本：

```
$ tar xvzf TDengine-enterprise-server-2.4.0.7-Linux-x64.tar.gz
TDengine-enterprise-server-2.4.0.7/
TDengine-enterprise-server-2.4.0.7/driver/
TDengine-enterprise-server-2.4.0.7/driver/vercomp.txt
TDengine-enterprise-server-2.4.0.7/driver/libtaos.so.2.4.0.7
TDengine-enterprise-server-2.4.0.7/install.sh
TDengine-enterprise-server-2.4.0.7/examples/
...

$ ll
total 43816
drwxrwxr-x  3 ubuntu ubuntu     4096 Feb 22 09:31 ./
drwxr-xr-x 20 ubuntu ubuntu     4096 Feb 22 09:30 ../
drwxrwxr-x  4 ubuntu ubuntu     4096 Feb 22 09:30 TDengine-enterprise-server-2.4.0.7/
-rw-rw-r--  1 ubuntu ubuntu 44852544 Feb 22 09:31 TDengine-enterprise-server-2.4.0.7-Linux-x64.tar.gz

$ cd TDengine-enterprise-server-2.4.0.7/

 $ ll
total 40784
drwxrwxr-x  4 ubuntu ubuntu     4096 Feb 22 09:30 ./
drwxrwxr-x  3 ubuntu ubuntu     4096 Feb 22 09:31 ../
drwxrwxr-x  2 ubuntu ubuntu     4096 Feb 22 09:30 driver/
drwxrwxr-x 10 ubuntu ubuntu     4096 Feb 22 09:30 examples/
-rwxrwxr-x  1 ubuntu ubuntu    33294 Feb 22 09:30 install.sh*
-rw-rw-r--  1 ubuntu ubuntu 41704288 Feb 22 09:30 taos.tar.gz

$ sudo ./install.sh

Start to update TDengine...
Created symlink /etc/systemd/system/multi-user.target.wants/taosd.service → /etc/systemd/system/taosd.service.
Nginx for TDengine is updated successfully!

To configure TDengine : edit /etc/taos/taos.cfg
To configure Taos Adapter (if has) : edit /etc/taos/taosadapter.toml
To start TDengine     : sudo systemctl start taosd
To access TDengine    : use taos -h shuduo-1804 in shell OR from http://127.0.0.1:6060

TDengine is updated successfully!
Install taoskeeper as a standalone service
taoskeeper is installed, enable it by `systemctl enable taoskeeper`
```

说明：install.sh 安装脚本在执行过程中，会通过命令行交互界面询问一些配置信息。如果希望采取无交互安装方式，那么可以用 -e no 参数来执行 install.sh 脚本。运行 ./install.sh -h 指令可以查看所有参数的详细说明信息。

### tar.gz 安装后的卸载

卸载命令如下:

```
$ rmtaos
Nginx for TDengine is running, stopping it...
TDengine is removed successfully!

taosKeeper is removed successfully!
```

## 安装目录说明

TDengine成功安装后，主安装目录是/usr/local/taos，目录内容如下：

```
$ cd /usr/local/taos
$ ll
$ ll
total 28
drwxr-xr-x  7 root root 4096 Feb 22 09:34 ./
drwxr-xr-x 12 root root 4096 Feb 22 09:34 ../
drwxr-xr-x  2 root root 4096 Feb 22 09:34 bin/
drwxr-xr-x  2 root root 4096 Feb 22 09:34 cfg/
lrwxrwxrwx  1 root root   13 Feb 22 09:34 data -> /var/lib/taos/
drwxr-xr-x  2 root root 4096 Feb 22 09:34 driver/
drwxr-xr-x 10 root root 4096 Feb 22 09:34 examples/
drwxr-xr-x  2 root root 4096 Feb 22 09:34 include/
lrwxrwxrwx  1 root root   13 Feb 22 09:34 log -> /var/log/taos/
```

- 自动生成配置文件目录、数据库目录、日志目录。
- 配置文件缺省目录：/etc/taos/taos.cfg， 软链接到/usr/local/taos/cfg/taos.cfg；
- 数据库缺省目录：/var/lib/taos， 软链接到/usr/local/taos/data；
- 日志缺省目录：/var/log/taos， 软链接到/usr/local/taos/log；
- /usr/local/taos/bin目录下的可执行文件，会软链接到/usr/bin目录下；
- /usr/local/taos/driver目录下的动态库文件，会软链接到/usr/lib目录下；
- /usr/local/taos/include目录下的头文件，会软链接到到/usr/include目录下；

## 卸载和更新文件说明

卸载安装包的时候，将保留配置文件、数据库文件和日志文件，即 /etc/taos/taos.cfg 、 /var/lib/taos 、 /var/log/taos 。如果用户确认后不需保留，可以手工删除，但一定要慎重，因为删除后，数据将永久丢失，不可以恢复！

如果是更新安装，当缺省配置文件（ /etc/taos/taos.cfg ）存在时，仍然使用已有的配置文件，安装包中携带的配置文件修改为taos.cfg.org保存在 /usr/local/taos/cfg/ 目录，可以作为设置配置参数的参考样例；如果不存在配置文件，就使用安装包中自带的配置文件。

## 注意事项

- TDengine提供了多种安装包，但最好不要在一个系统上同时使用 tar.gz 安装包和 deb 或 rpm 安装包。否则会相互影响，导致在使用时出现问题。

- 对于deb包安装后，如果安装目录被手工误删了部分，出现卸载、或重新安装不能成功。此时，需要清除 tdengine 包的安装信息，执行如下命令：

```
$ sudo rm -f /var/lib/dpkg/info/tdengine*
```

然后再重新进行安装就可以了。

- 对于rpm包安装后，如果安装目录被手工误删了部分，出现卸载、或重新安装不能成功。此时，需要清除tdengine包的安装信息，执行如下命令：

```
$ sudo rpm -e --noscripts tdengine
```

然后再重新进行安装就可以了。
