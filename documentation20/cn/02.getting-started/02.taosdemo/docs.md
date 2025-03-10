# 如何使用 taosBenchmark 进行性能测试

自从 TDengine 2019年 7 月开源以来，凭借创新的数据建模设计、快捷的安装方式、易用的编程接口和强大的数据写入查询性能博得了大量时序数据开发者的青睐。其中写入和查询性能往往令刚接触 TDengine 的用户称叹不已。为了便于用户在最短时间内就可以体验到 TDengine 的高性能特点，我们专门开发了一个应用程序 taosBenchmark （曾命名为 taosdemo）用于对 TDengine 进行写入和查询的性能测试，用户可以通过 taosBenchmark 轻松模拟大量设备产生海量数据的场景，并且可以通过 taosBenchmark 参数灵活控制表的列数、数据类型、乱序比例以及并发线程数量。

运行 taosBenchmark 很简单，通过下载 [TDengine 安装包](https://www.taosdata.com/cn/all-downloads/)或者自行下载 [TDengine 代码](https://github.com/taosdata/TDengine)编译都可以在安装目录或者编译结果目录中找到并运行。

接下来本文为大家讲解 taosBenchmark 的使用介绍及注意事项。

## 使用 taosBenchmark 进行写入测试

不使用任何参数的情况下执行 taosBenchmark 命令，输出如下：

```
$ taosBenchmark

taosBenchmark is simulating data generated by power equipment monitoring...

host:                       127.0.0.1:6030
user:                       root
password:                   taosdata
configDir:
resultFile:                 ./output.txt
thread num of insert data:  8
thread num of create table: 8
top insert interval:        0
number of records per req:  30000
max sql length:             1048576
database count:             1
database[0]:
  database[0] name:      test
  drop:                  yes
  replica:               1
  precision:             ms
  super table count:     1
  super table[0]:
      stbName:           meters
      autoCreateTable:   no
      childTblExists:    no
      childTblCount:     10000
      childTblPrefix:    d
      dataSource:        rand
      iface:             taosc
      insertRows:        10000
      interlaceRows:     0
      disorderRange:     1000
      disorderRatio:     0
      maxSqlLen:         1048576
      timeStampStep:     1
      startTimestamp:    2017-07-14 10:40:00.000
      sampleFormat:
      sampleFile:
      tagsFile:
      columnCount:       3
column[0]:FLOAT column[1]:INT column[2]:FLOAT
      tagCount:            2
        tag[0]:INT tag[1]:BINARY(16)

         Press enter key to continue or Ctrl-C to stop
```

这里显示的是接下来 taosBenchmark 进行数据写入的各项参数。默认不输入任何命令行参数的情况下 taosBenchmark 将模拟生成一个电力行业典型应用的电表数据采集场景数据。即建立一个名为 test 的数据库，并创建一个名为 meters 的超级表，其中表结构为：

```
taos> describe test.meters;
             Field              |         Type         |   Length    |   Note   |
=================================================================================
 ts                             | TIMESTAMP            |           8 |          |
 current                        | FLOAT                |           4 |          |
 voltage                        | INT                  |           4 |          |
 phase                          | FLOAT                |           4 |          |
 groupid                        | INT                  |           4 | TAG      |
 location                       | BINARY               |          64 | TAG      |
Query OK, 6 row(s) in set (0.002972s)
```

按任意键后 taosBenchmark 将建立数据库 test 和超级表 meters，并按照 TDengine 数据建模的最佳实践，以 meters 超级表为模板生成一万个子表，代表一万个独立上报数据的电表设备。

```
taos> use test;
Database changed.

taos> show stables;
              name              |      created_time       | columns |  tags  |   tables    |
============================================================================================
 meters                         | 2021-08-27 11:21:01.209 |       4 |      2 |       10000 |
Query OK, 1 row(s) in set (0.001740s)
```

然后 taosBenchmark 为每个电表设备模拟生成一万条记录：

```
...
====thread[3] completed total inserted rows: 6250000, total affected rows: 6250000. 347626.22 records/second====
[1]:100%
====thread[1] completed total inserted rows: 6250000, total affected rows: 6250000. 347481.98 records/second====
[4]:100%
====thread[4] completed total inserted rows: 6250000, total affected rows: 6250000. 347149.44 records/second====
[8]:100%
====thread[8] completed total inserted rows: 6250000, total affected rows: 6250000. 347082.43 records/second====
[6]:99%
[6]:100%
====thread[6] completed total inserted rows: 6250000, total affected rows: 6250000. 345586.35 records/second====
Spent 18.0863 seconds to insert rows: 100000000, affected rows: 100000000 with 16 thread(s) into test.meters. 5529049.90 records/second

insert delay, avg:      28.64ms, max:     112.92ms, min:       9.35ms
```

以上信息是在一台具备 8个CPU 64G 内存的普通 PC 服务器上进行实测的结果。显示 taosBenchmark 用了 18 秒的时间插入了 100000000 （一亿）条记录，平均每秒钟插入 552 万 9千零49 条记录。

TDengine 还提供性能更好的参数绑定接口，而在同样的硬件上使用参数绑定接口 （taosBenchmark -I stmt ）进行相同数据量的写入，结果如下：

```
...

====thread[14] completed total inserted rows: 6250000, total affected rows: 6250000. 1097331.55 records/second====
[9]:97%
[4]:97%
[3]:97%
[3]:98%
[4]:98%
[9]:98%
[3]:99%
[4]:99%
[3]:100%
====thread[3] completed total inserted rows: 6250000, total affected rows: 6250000. 1089038.19 records/second====
[9]:99%
[4]:100%
====thread[4] completed total inserted rows: 6250000, total affected rows: 6250000. 1087123.09 records/second====
[9]:100%
====thread[9] completed total inserted rows: 6250000, total affected rows: 6250000. 1085689.38 records/second====
[11]:91%
[11]:92%
[11]:93%
[11]:94%
[11]:95%
[11]:96%
[11]:97%
[11]:98%
[11]:99%
[11]:100%
====thread[11] completed total inserted rows: 6250000, total affected rows: 6250000. 1039087.65 records/second====
Spent 6.0257 seconds to insert rows: 100000000, affected rows: 100000000 with 16 thread(s) into test.meters. 16595590.52 records/second

insert delay, avg:       8.31ms, max:     860.12ms, min:       2.00ms
```

显示 taosBenchmark 用了 6 秒的时间插入了一亿条记录，每秒钟插入性能高达 1659 万 5 千 590 条记录。

由于 taosBenchmark 使用起来非常方便，我们又对 taosBenchmark 做了更多的功能扩充，使其支持更复杂的参数设置，便于进行快速原型开发的样例数据准备和验证工作。

完整的 taosBenchmark 命令行参数列表可以通过 taosBenchmark --help 显示如下：

```
$ taosBenchmark --help

-f, --file=FILE The meta file to the execution procedure. Currently, we support standard UTF-8 (without BOM) encoded files only.
-u, --user=USER The user name to use when connecting to the server.
-p, --password The password to use when connecting to the server.
-c, --config-dir=CONFIG_DIR Configuration directory.
-h, --host=HOST TDengine server FQDN to connect. The default host is localhost.
-P, --port=PORT The TCP/IP port number to use for the connection.
-I, --interface=INTERFACE The interface (taosc, rest, and stmt) taosBenchmark uses. By default use 'taosc'.
-d, --database=DATABASE Destination database. By default is 'test'.
-a, --replica=REPLICA Set the replica parameters of the database, By default use 1, min: 1, max: 3.
-m, --table-prefix=TABLEPREFIX Table prefix name. By default use 'd'.
-s, --sql-file=FILE The select sql file.
-N, --normal-table Use normal table flag.
-o, --output=FILE Direct output to the named file. By default use './output.txt'.
-q, --query-mode=MODE Query mode -- 0: SYNC, 1: ASYNC. By default use SYNC.
-b, --data-type=DATATYPE The data_type of columns, By default use: FLOAT, INT, FLOAT.
-w, --binwidth=WIDTH The width of data_type 'BINARY' or 'NCHAR'. By default use 64
-l, --columns=COLUMNS The number of columns per record. Demo mode by default is 1 (float, int, float). Max values is 4095
All of the new column(s) type is INT. If use -b to specify column type, -l will be ignored.
-T, --threads=NUMBER The number of threads. By default use 8.
-i, --insert-interval=NUMBER The sleep time (ms) between insertion. By default is 0.
-S, --time-step=TIME_STEP The timestamp step between insertion. By default is 1.
-B, --interlace-rows=NUMBER The interlace rows of insertion. By default is 0.
-r, --rec-per-req=NUMBER The number of records per request. By default is 30000.
-t, --tables=NUMBER The number of tables. By default is 10000.
-n, --records=NUMBER The number of records per table. By default is 10000.
-M, --random The value of records generated are totally random.
By default to simulate power equipment scenario.
-x, --aggr-func Test aggregation functions after insertion.
-y, --answer-yes Input yes for prompt.
-O, --disorder=NUMBER Insert order mode--0: In order, 1 ~ 50: disorder ratio. By default is in order.
-R, --disorder-range=NUMBER Out of order data's range. Unit is ms. By default is 1000.
-g, --debug Print debug info.
-?, --help Give this help list
--usage Give a short usage message
-V, --version Print program version.

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to <support@taosdata.com>.
```

taosBenchmark 的参数是为了满足数据模拟的需求来设计的。下面介绍几个常用的参数：

```
-I, --interface=INTERFACE     The interface (taosc, rest, and stmt) taosBenchmark uses. Default is 'taosc'.
```

前面介绍 taosBenchmark 不同接口的性能差异已经提到， -I 参数为选择不同的接口，目前支持 taosc、stmt 和 rest 几种。其中 taosc 为使用 SQL 语句方式进行数据写入；stmt 为使用参数绑定接口进行数据写入；rest 为使用 RESTful 协议进行数据写入。

```
-T, --threads=NUMBER          The number of threads. Default is 8.
```

-T 参数设置 taosBenchmark 使用多少个线程进行数据同步写入，通过多线程可以尽最大可能压榨硬件的处理能力。

```
-b, --data-type=DATATYPE      The data_type of columns, default: FLOAT, INT, FLOAT.

-w, --binwidth=WIDTH          The width of data_type 'BINARY' or 'NCHAR'. Default is 64

-l, --columns=COLUMNS         The number of columns per record. Demo mode by default is 3 (float, int, float). Max values is 4095
```

前文提到，taosBenchmark 默认创建一个典型电表数据采集应用场景，每个设备包含电流电压相位3个采集量。对于需要定义不同的采集量，可以使用 -b 参数。TDengine 支持 BOOL、TINYINT、SMALLINT、INT、BIGINT、FLOAT、DOUBLE、BINARY、NCHAR、TIMESTAMP 等多种数据类型。通过 -b 加上以“ , ”（英文逗号）分割定制类型的列表可以使 taosBenchmark 建立对应的超级表和子表并插入相应模拟数据。通过 -w 参数可以指定 BINARY 和 NCHAR 数据类型的列的宽度（默认为 64 ）。-l 参数可以在 -b 参数指定数据类型的几列之后补充以 INT 型的总的列数，特别多列的情况下可以减少手工输入的过程，最多支持到 4095 列。

```
-r, --rec-per-req=NUMBER      The number of records per request. Default is 30000.
```

为了达到 TDengine 性能极限，可以使用多客户端、多线程以及一次插入多条数据来进行数据写入。 -r 参数为设置一次写入请求可以拼接的记录条数，默认为30000条。有效的拼接记录条数还和客户端缓冲区大小有关，目前的缓冲区为 1M Bytes，如果记录的列宽度比较大，最大拼接记录条数可以通过 1M 除以列宽（以字节为单位）计算得出。

```
-t, --tables=NUMBER           The number of tables. Default is 10000.
-n, --records=NUMBER          The number of records per table. Default is 10000.
-M, --random                  The value of records generated are totally random. The default is to simulate power equipment senario.
```

前面提到 taosBenchmark 默认创建 10000 个表，每个表写入 10000 条记录。可以通过 -t 和 -n 设置表的数量和每个表的记录的数量。默认无参数生成的数据为模拟真实场景，模拟生成的数据为电流电压相位值增加一定的抖动，可以更真实表现 TDengine 高效的数据压缩能力。如果需要模拟生成完全随机数据，可以通过 -M 参数。

```
-y, --answer-yes              Default input yes for prompt.
```

前面我们可以看到 taosBenchmark 默认在进行创建数据库或插入数据之前输出将要进行操作的参数列表，方便使用者在插入之前了解即将进行的数据写入的内容。为了方便进行自动测试，-y 参数可以使 taosBenchmark 输出参数后立刻进行数据写入操作。

```
-O, --disorder=NUMBER         Insert order mode--0: In order, 1 ~ 50: disorder ratio. Default is in order.
-R, --disorder-range=NUMBER   Out of order data's range, ms, default is 1000.
```

在某些场景，接收到的数据并不是完全按时间顺序到来，而是包含一定比例的乱序数据，TDengine 也能进行很好的处理。为了模拟乱序数据的写入，taosBenchmark 提供 -O 和 -R 参数进行设置。-O 参数为 0 和不使用 -O 参数相同为完全有序数据写入。1 到 50 为数据中包含乱序数据的比例。-R 参数为乱序数据时间戳偏移的范围，默认为 1000 毫秒。另外注意，时序数据以时间戳为唯一标识，所以乱序数据可能会生成和之前已经写入数据完全相同的时间戳，这样的数据会根据数据库创建的 update 值或者被丢弃（update 0）或者覆盖已有数据（update 1 或 2），而总的数据条数可能和期待的条数不一致的情况。

```
 -g, --debug                   Print debug info.
```

如果对 taosBenchmark 写入数据过程感兴趣或者数据写入结果不符合预期，可以使用 -g 参数使 taosBenchmark 打印执行过程中间调试信息到屏幕上，或通过 Linux 重定向命令导入到另外一个文件，方便找到发生问题的原因。另外 taosBenchmark 在执行失败后也会把相应执行的语句和调试原因输出到屏幕。可以搜索 reason 来找到 TDengine 服务端返回的错误原因信息。

```
-x, --aggr-func               Test aggregation funtions after insertion.
```

TDengine 不仅仅是插入性能非常强大，由于其先进的数据库引擎设计使查询性能也异常强大。taosBenchmark 提供一个 -x 函数，可以在插入数据结束后进行常用查询操作并输出查询消耗时间。以下为在前述服务器上进行插入一亿条记录后进行常用查询的结果。

可以看到 select * 取出一亿条记录（不输出到屏幕）操作仅消耗1.26秒。而对一亿条记录进行常用的聚合函数操作通常仅需要二十几毫秒，时间最长的 count 函数也不到四十毫秒。

```
taosBenchmark -I stmt -T 48 -y -x
...
...
select          * took 1.266835 second(s)
...
select   count(*) took 0.039684 second(s)
...
Where condition: groupid = 1
select avg(current) took 0.025897 second(s)
...
select sum(current) took 0.025622 second(s)
...
select max(current) took 0.026124 second(s)
...
...
select min(current) took 0.025812 second(s)
...
select first(current) took 0.024105 second(s)
...
```

除了命令行方式， taosBenchmark 还支持接受指定一个 JSON 文件做为传入参数的方式来提供更丰富的设置。一个典型的 JSON 文件内容如下：

```
{
    "filetype": "insert",
    "cfgdir": "/etc/taos",
    "host": "127.0.0.1",
    "port": 6030,
    "user": "root",
    "password": "taosdata",
    "thread_count": 4,
    "thread_count_create_tbl": 4,
    "result_file": "./insert_res.txt",
    "confirm_parameter_prompt": "no",
    "insert_interval": 0,
    "interlace_rows": 100,
    "num_of_records_per_req": 100,
    "databases": [{
        "dbinfo": {
            "name": "db",
            "drop": "yes",
            "replica": 1,
            "days": 10,
            "cache": 16,
            "blocks": 8,
            "precision": "ms",
            "keep": 3650,
            "minRows": 100,
            "maxRows": 4096,
            "comp":2,
            "walLevel":1,
            "cachelast":0,
            "quorum":1,
            "fsync":3000,
            "update": 0
        },
        "super_tables": [{
            "name": "stb",
            "child_table_exists":"no",
            "childtable_count": 100,
            "childtable_prefix": "stb_",
            "auto_create_table": "no",
            "batch_create_tbl_num": 5,
            "data_source": "rand",
            "insert_mode": "taosc",
            "insert_rows": 100000,
            "childtable_limit": 10,
            "childtable_offset":100,
            "interlace_rows": 0,
            "insert_interval":0,
            "max_sql_len": 1024000,
            "disorder_ratio": 0,
            "disorder_range": 1000,
            "timestamp_step": 10,
            "start_timestamp": "2020-10-01 00:00:00.000",
            "sample_format": "csv",
            "sample_file": "./sample.csv",
            "tags_file": "",
            "columns": [{"type": "INT"}, {"type": "DOUBLE", "count":10}, {"type": "BINARY", "len": 16, "count":3}, {"type": "BINARY", "len": 32, "count":6}],
            "tags": [{"type": "TINYINT", "count":2}, {"type": "BINARY", "len": 16, "count":5}]
        }]
    }]
}
```

例如：我们可以通过 "thread_count" 和 "thread_count_create_tbl" 来为建表和插入数据指定不同数量的线程。可以通过 "child_table_exists"、"childtable_limit" 和 "childtable_offset" 的组合来使用多个 taosBenchmark 进程（甚至可以在不同的电脑上）对同一个超级表的不同范围子表进行同时写入。也可以通过 "data_source" 和 "sample_file" 来指定数据来源为 csv 文件，来实现导入已有数据的功能。

## 使用 taosBenchmark 进行查询和订阅测试

taosBenchmark 不仅仅可以进行数据写入，也可以执行查询和订阅功能。但一个 taosBenchmark 实例只能支持其中的一种功能，不能同时支持三种功能，通过配置文件来指定进行哪种功能的测试。

以下为一个典型查询 JSON 示例文件内容：

```
{
  "filetype": "query",
  "cfgdir": "/etc/taos",
  "host": "127.0.0.1",
  "port": 6030,
  "user": "root",
  "password": "taosdata",
  "confirm_parameter_prompt": "no",
  "databases": "db",
  "query_times": 2,
  "query_mode": "taosc",
  "specified_table_query": {
    "query_interval": 1,
    "concurrent": 3,
    "sqls": [
      {
        "sql": "select last_row(*) from stb0 ",
        "result": "./query_res0.txt"
      },
      {
        "sql": "select count(*) from stb00_1",
        "result": "./query_res1.txt"
      }
    ]
  },
  "super_table_query": {
    "stblname": "stb1",
    "query_interval": 1,
    "threads": 3,
    "sqls": [
      {
        "sql": "select last_row(ts) from xxxx",
        "result": "./query_res2.txt"
      }
    ]
  }
}
```

以下为 JSON 文件中和查询相关的特有参数含义：

```
"query_times": 每种查询类型的查询次数
"query_mode": 查询数据接口，"taosc"：调用TDengine的c接口；“resetful”：使用restfule接口。可选项。缺省是“taosc”。
"specified_table_query": { 指定表的查询
"query_interval": 执行sqls的间隔，单位是秒。可选项，缺省是0。
"concurrent": 并发执行sqls的线程数，可选项，缺省是1。每个线程都执行所有的sqls。
"sqls": 可以添加多个sql语句，最多支持100条。
"sql": 查询语句。必选项。
"result": 查询结果写入的文件名。可选项，缺省是空，表示查询结果不写入文件。
"super_table_query": { 对超级表中所有子表的查询
"stblname": 超级表名称。必选项。
"query_interval": 执行sqls的间隔，单位是秒。可选项，缺省是0。
"threads": 并发执行sqls的线程数，可选项，缺省是1。每个线程负责一部分子表，执行所有的sqls。
"sql": "select count(*) from xxxx"。查询超级表内所有子表的查询语句，其中表名必须写成 “xxxx”，实例会自动替换成子表名。
"result": 查询结果写入的文件名。可选项，缺省是空，表示查询结果不写入文件。
```

以下为一个典型订阅 JSON 示例文件内容：

```
{
    "filetype":"subscribe",
    "cfgdir": "/etc/taos",
    "host": "127.0.0.1",
    "port": 6030,
    "user": "root",
    "password": "taosdata",
    "databases": "db",
    "confirm_parameter_prompt": "no",
    "specified_table_query":
      {
       "concurrent":1,
       "mode":"sync",
       "interval":0,
       "restart":"yes",
       "keepProgress":"yes",
       "sqls": [
        {
          "sql": "select * from stb00_0 ;",
          "result": "./subscribe_res0.txt"
        }]
      },
    "super_table_query":
      {
       "stblname": "stb0",
       "threads":1,
       "mode":"sync",
       "interval":10000,
       "restart":"yes",
       "keepProgress":"yes",
       "sqls": [
        {
          "sql": "select * from xxxx where ts > '2021-02-25 11:35:00.000' ;",
          "result": "./subscribe_res1.txt"
        }]
      }
  }
```

以下为订阅功能相关的特有参数含义：

```
"interval": 执行订阅的间隔，单位是秒。可选项，缺省是0。
"restart": 订阅重启。"yes"：如果订阅已经存在，重新开始，"no": 继续之前的订阅。(请注意执行用户需要对 dataDir 目录有读写权限)
"keepProgress": 保留订阅信息进度。yes表示保留订阅信息，no表示不保留。该值为yes，restart为no时，才能继续之前的订阅。
"resubAfterConsume": 配合 keepProgress 使用，在订阅消费了相应次数后调用 unsubscribe 取消订阅并再次订阅。
"result": 查询结果写入的文件名。可选项，缺省是空，表示查询结果不写入文件。 注意：每条sql语句后的保存结果的文件不能重名，且生成结果文件时，文件名会附加线程号。
```

## 结语

TDengine是涛思数据专为物联网、车联网、工业互联网、IT运维等设计和优化的大数据平台。TDengine 由于数据库内核中创新的数据存储和查询引擎设计，展现出远超同类产品的高效性能。并且由于支持 SQL 语法和多种编程语言的连接器（目前支持 Java, Python, Go, C#, NodeJS, Rust 等），易用性极强，学习成本为零。为了便于运维需求，我们还提供数据迁移和监控功能等相关生态工具软件。

为了刚接触 TDengine 的使用者方便进行技术评估和压力测试，我们为 taosBenchmark 开发了丰富的特性。本文即为对 taosBenchmark 的一个简单介绍，随着 TDengine 新功能的不断增加，taosBenchmark 也会继续演化和改进。taosBenchmark 的代码做为 TDengine 的一部分在 GitHub 上完全开源。欢迎就 taosBenchmark 或 TDengine 的使用或实现在 GitHub 或者涛思数据的用户群提出建议或批评。

## 附录 - 完整 taosBenchmark 参数介绍

taosBenchmark支持两种配置参数的模式，一种是命令行参数，一种是使用 JSON 格式的配置文件。

一、命令行参数

-f：指定taosBenchmark所需参数的meta文件。当使用该参数时，其他所有命令行参数都失效。可选项，缺省是NULL。目前仅支持不含 BOM（byte-order mark）的标准 UTF-8 编码文件。

-u： 用户名。可选项，缺省是“root“。

-p： 密码。可选项，缺省是“taosdata"。指定密码需要使用 MySQL 风格，即密码和 -p 贴紧方式，中间无空格。

-c： 配置文件taos.cfg所在的路径。因为taosBenchmark通过包含taos的动态库，去链接taosd服务，所以需要做好配置文件。可选项，缺省是 "/etc/taos"路径。

-h：taosd服务的FQDN。可选项，缺省是“localhost“。

-P： taosd服务的端口号。可选项，缺省是6030。

-d：数据库名称。可选项，缺省是“test”。

-a：副本个数，可选项。1 - 3，缺省是1。

-m：表名的前缀。可选项，缺省是“t”。

-s：：执行该文件包含的多条 SQL 查询命令。

-N：使用普通建表模式。有该选项时，全部创建普通表，否则缺省创建超级表，再根据超级表创建子表；

-o：指定执行结果输出文件的路径。可选项，缺省是./output.txt。

-q：查询模式，0：同步模式；1：异步模式。可选项，缺省是0。

-b：列的类型。可选项，缺省是：FLOAT,INT,FLOAT。NCHAR和BINARY也可以自定义长度，例如: NCHAR(16), BINARY(8)

-w：BINARY或NCHAR数据类型的长度。可选项，缺省是16。

-l：列的个数。可选项，缺省是3。

-T：并发线程数。可选项，缺省是10。

-i：两次sql插入的休眠时间间隔，缺省是0。

-S：两次插入间隔时间戳步长，缺省是1。

-B：交错（interlace）写入模式，缺省是0（顺序写入模式）。

-r：每条插入请求包含的记录数，缺省是30000。

-t：表的个数。可选项，缺省是10000。

-n：每个表插入的记录数。可选项，缺省是10000。

-M: 插入数据为完全随机。可选项，缺省为模拟能源设备真实场景（数据在固定范围小幅波动）。

-x：不仅仅插入数据。有该选项时，taosBenchmark还会进行聚合函数查询操作。

-y：提示询问输入时缺省输入yes。

-O：插入乱序数据的比例，0：顺序插入；> 0：乱序数据的百分比。可选项，缺省是0。、

-R：乱序百分比不为0时，乱序时间戳范围，单位：毫秒。可选项，缺省是1000。

-g：打印debug信息

-V： 打印taosBenchmark的debug信息。

--help: 打印命令参数列表。

二、JSON 格式的配置文件中所有参数说明

taosBenchmark支持3种功能的测试，包括插入、查询、订阅。但一个taosBenchmark实例不能同时支持三种功能，一个 taosBenchmark 实例只能支持其中的一种功能，通过配置文件来指定进行哪种功能的测试。

1、插入功能测试的 JSON 配置文件

```
{
    "filetype": "insert",
    "cfgdir": "/etc/taos",
    "host": "127.0.0.1",
    "port": 6030,
    "user": "root",
    "password": "taosdata",
    "thread_count": 4,
    "thread_count_create_tbl": 4,
    "result_file": "./insert_res.txt",
    "confirm_parameter_prompt": "no",
    "insert_interval": 0,
    "interlace_rows": 100,
    "num_of_records_per_req": 100,
    "databases": [{
        "dbinfo": {
            "name": "db",
            "drop": "yes",
            "replica": 1,
            "days": 10,
            "cache": 16,
            "blocks": 8,
            "precision": "ms",
            "keep": 3650,
            "minRows": 100,
            "maxRows": 4096,
            "comp":2,
            "walLevel":1,
            "cachelast":0,
            "quorum":1,
            "fsync":3000,
            "update": 0
        },
        "super_tables": [{
            "name": "stb",
            "child_table_exists":"no",
            "childtable_count": 100,
            "childtable_prefix": "stb_",
            "auto_create_table": "no",
            "batch_create_tbl_num": 5,
            "data_source": "rand",
            "insert_mode": "taosc",
            "insert_rows": 100000,
            "childtable_limit": 10,
            "childtable_offset":100,
            "interlace_rows": 0,
            "insert_interval":0,
            "max_sql_len": 1024000,
            "disorder_ratio": 0,
            "disorder_range": 1000,
            "timestamp_step": 10,
            "start_timestamp": "2020-10-01 00:00:00.000",
            "sample_format": "csv",
            "sample_file": "./sample.csv",
               "use_sameple_ts": "no",
            "tags_file": "",
            "columns": [{"type": "INT"}, {"type": "DOUBLE", "count":10}, {"type": "BINARY", "len": 16, "count":3}, {"type": "BINARY", "len": 32, "count":6}],
            "tags": [{"type": "TINYINT", "count":2}, {"type": "BINARY", "len": 16, "count":5}]
        }]
    }]
}
```

"filetype": 本taosBenchmark实例进行哪种功能测试。"insert"表示数据插入功能。必选项。

"cfgdir": 配置文件taos.cfg所在的路径。因为taosBenchmark通过包含taos的动态库，去链接taosd服务，所以需要做好配置文件。可选项，缺省是 "/etc/taos"路径。

"host": taosd服务的FQDN。可选项，缺省是“localhost“。

"port": taosd服务的端口号。可选项，缺省是6030。

"user": 用户名。可选项，缺省是“root“。

"password": 密码。可选项，缺省是“taosdata"。

"thread_count": 插入数据时的并发线程数。可选项，缺省是1。

"thread_count_create_tbl": 建子表时的并发线程数。可选项，缺省是1。

"result_file": 测试完成后结果保存文件。可选项，缺省是本实例启动目录下的"./insert_res.txt"。

"confirm_parameter_prompt": 执行过程中提示是否确认，为no时，执行过程无需手工输入enter。可选项，缺省是no。

"insert_interval": 两次发送请求的间隔时间。可选项，缺省是0，代表无人工设置的时间间隔，单位为ms。。

"interlace_rows": 设置轮询插入每个单表数据的条目数，如果interlace_rows*childtable_count*supertable_num小于num_of_records_per_req时，则请求插入的数目以interlace_rows*childtable_count*supertable_num为准。可选项，缺省是0。

"num_of_records_per_req": 每条请求数据内容包含的插入数据记录数目，该数据组成的sql不能大于maxsqllen，如果过大，则取taosd限制的1M长度(1048576)。0代表不插入数据，建议配置大于0。

"databases": [{

"dbinfo": {"name": 数据库名称。必选项。

"drop": 如果数据库已经存在，”yes“：删除后重建；”no“：不删除，直接使用。可选项，缺省是”no“。drop = yes 会使其他子表创建相关条目无效。

"replica": 副本个数，可选项。1 - 3，缺省是1。

"days": 数据文件存储数据的时间跨度，可选项。缺省是10天。

"cache": 内存块的大小，单位是MB，可选项。缺省是16MB。

"blocks": 每个VNODE（TSDB）中有多少cache大小的内存块，可选项。缺省是6块。

"precision": 数据库时间精度，可选项。"ms"：毫秒， “us”：微秒。缺省是“ms”。in

"keep": 数据保留的天数，可选项。缺省是3650天。

"minRows": 文件块中记录的最小条数，可选项。缺省是100。

"maxRows": 文件块中记录的最大条数，可选项。缺省是4096.

"comp":文件压缩标志位，可选项。0：关闭，1:一阶段压缩，2:两阶段压缩。缺省是2。

"walLevel":WAL级别，可选项。1：写wal, 但不执行fsync; 2：写wal, 而且执行fsync。缺省是1。

"cachelast":允许在内存中保留每张表的最后一条记录。1表示允许。

"quorum":异步写入成功所需应答之法定数，1-3，可选项。缺省是1。

"fsync":当wal设置为2时，执行fsync的周期，单位是毫秒，最小为0，表示每次写入，立即执行fsync. 最大为180000，可选项。缺省是3000。

"update": 支持数据更新，0：否；1：是。可选项。缺省是0。 ​ },

"super_tables": [{ ​ "name": 超级表名称，必选项。

"child_table_exists": 子表是否已经存在，“yes”：是；"no"：否。指定“是”后，不再建子表，后面相关子表的参数就无效了。可选项，缺省是“no”。database 设置 drop = yes 时，无论配置文件内容，此参数将自动置为 no。

"childtable_count": 建立子表个数 。该值需要大于0。当child_table_exists为“no”时，必选项，否则就是无效项。

"childtable_prefix": 子表名称前缀。当child_table_exists为“no”时，必选项，否则就是无效项。确保数据库中表名没有重复。

"auto_create_table": 子表的创建方式，“yes”：自动建表；"no"：提前建表。可选项，缺省是“no”。当 child_table_exists 为 “yes” 时此参数将自动置为 no 。

"batch_create_tbl_num": 一个sql批量创建子表的数目。

"data_source": 插入数据来源，"rand"：实例随机生成；“sample”：从样例文件中读取。可选项。缺省是“rand”。

"insert_mode": 插入数据接口，"taosc"：调用TDengine的c接口；“rest”：使用restful接口；“stmt”：使用 stmt （参数绑定）接口 （目前仅在 develop 分支代码中）。可选项。缺省是“taosc”。

"insert_rows": 插入记录数，0：一直插入，永不退出；>0：每个子表插入记录数，完成后实例退出。可选项，缺省是0。

"childtable_offset": 插入数据时，子表起始值。只在drop=no && child_table_exists= yes，该字段生效。

"childtable_limit": 插入数据时，子表从offset开始，偏移的表数目。使用者可以运行多个 taosBenchmark 实例（甚至可以在不同的机器上）通过使用不同的 childtable_offset 和 childtable_limit 配置值来实现同时写入相同数据库相同超级表下多个子表。只在drop=no && child_table_exists= yes，该字段生效。

"interlace_rows": 跟上面的配置一致，不过该处的配置优先，每个stable可以有自己单独的配置。最大不超过 num_of_records_per_req。

"insert_interval": 同上。

"max_sql_len": 同上</u>

"disorder_ratio": 插入数据时的乱序百分比，可选项，缺省是0。

"disorder_range": 乱序百分比不为0时，乱序时间戳范围，单位：ms。可选项，缺省是1000，即1秒或1000毫秒。

"timestamp_step": 每个子表中记录时间戳的步长，单位：ms。可选项，缺省是1，即1毫秒。

"start_timestamp": 子表中记录时间戳的起始值，支持"2020-10-01 00:00:00.000"和“now”两种格式，可选项，缺省是“now”。

"sample_format": 当插入数据源选择“sample”时，sample文件的格式，"csv"：csv格式，每列的值与子表的columns保持一致，但不包含第1列的时间戳。可选项，缺省是”csv”。目前仅仅支持csv格式的sample文件。

"sample_file":sample文件，包含路径和文件名。当插入数据源选择“sample”时，该项为必选项。

"use_sample_ts":sample文件是否包含第一列时间戳，可选项: "yes" 和 "no", 默认 "no"。(注意：若为yes，则disorder_ratio 和 disorder_range失效)

"tags_file": 子表tags值文件，只能是csv文件格式，且必须与超级表的tags保持一致。当该项为非空时，表示子表的tags值从文件中获取；为空时，实例随机生成。可选项，缺省是空。

"columns": [{ 超级表的column列表，最大支持1024列（指所有普通列+超级列总和）。默认的第一列为时间类型，程序自动添加，不需要手工添加。

"type": 该列的数据类型 ，必选项。

"len": 该列的长度，只有type是BINARY或NCHAR时有效，可选项，缺省值是8。

"count":该类型的连续列个数，可选项，缺省是1。

}],

"tags": [{ 超级表的tags列表，type不能是timestamp类型， 最大支持128个。

"type": 该列的数据类型 ，必选项。

"len": 该列的长度，只有type是BINARY或NCHAR时有效，可选项，缺省值是8。

"count":该类型的连续列个数，可选项，缺省是1。

}]

2、查询功能测试的 JSON 配置文件

```
{
  "filetype": "query",
  "cfgdir": "/etc/taos",
  "host": "127.0.0.1",
  "port": 6030,
  "user": "root",
  "password": "taosdata",
  "confirm_parameter_prompt": "no",
  "databases": "db",
  "query_times": 2,
  "query_mode": "taosc",
  "specified_table_query": {
    "query_interval": 1,
    "concurrent": 3,
    "sqls": [
      {
        "sql": "select last_row(*) from stb0 ",
        "result": "./query_res0.txt"
      },
      {
        "sql": "select count(*) from stb00_1",
        "result": "./query_res1.txt"
      }
    ]
  },
  "super_table_query": {
    "stblname": "stb1",
    "query_interval": 1,
    "threads": 3,
    "sqls": [
      {
        "sql": "select last_row(ts) from xxxx",
        "result": "./query_res2.txt"
      }
    ]
  }
}
```

"filetype": 本taosBenchmark实例进行哪种功能测试。"query"表示数据查询功能。必选项。

"cfgdir": 配置文件taos.cfg所在的路径。因为taosBenchmark通过包含taos的动态库，去链接taosd服务，所以需要做好配置文件。可选项，缺省是 "/etc/taos"路径。

"host": taosd服务的FQDN。可选项，缺省是“localhost“。

"port": taosd服务的端口号。可选项，缺省是6030。

"user": 用户名。可选项，缺省是“root“。

"password": 密码。可选项，缺省是“taosdata"。

"confirm_parameter_prompt": 执行过程中提示是否确认，为no时，执行过程无需手工输入enter。可选项，缺省是no。

"databases": 数据库名称。必选项。

"query_times": 每种查询类型的查询次数

"query_mode": 查询数据接口，"taosc"：调用TDengine的c接口；“resetful”：使用restfule接口。可选项。缺省是“taosc”。

"specified_table_query": { 指定表的查询

"query_interval": 执行sqls的间隔，单位是秒。可选项，缺省是0。

"concurrent": 并发执行sqls的线程数，可选项，缺省是1。每个线程都执行所有的sqls。

"sqls": 可以添加多个sql语句，最多支持100条。

"sql": 查询语句。必选项。

"result": 查询结果写入的文件名。可选项，缺省是空，表示查询结果不写入文件。

"super_table_query": { 对超级表中所有子表的查询

"stblname": 超级表名称。必选项。

"query_interval": 执行sqls的间隔，单位是秒。可选项，缺省是0。

"threads": 并发执行sqls的线程数，可选项，缺省是1。每个线程负责一部分子表，执行所有的sqls。

"sql": "select count(*) from xxxx"。查询超级表内所有子表的查询语句，其中表名必须写成 “xxxx”，实例会自动替换成子表名。

"result": 查询结果写入的文件名。可选项，缺省是空，表示查询结果不写入文件。

注意：每条sql语句后的保存结果的文件不能重名，且生成结果文件时，文件名会附加线程号。

查询结果显示：如果查询线程结束一次查询距开始执行时间超过30秒打印一次查询次数、用时和QPS。所有查询结束时，汇总打印总的查询次数和QPS。

3、订阅功能测试的 JSON 配置文件

```
{
    "filetype":"subscribe",
    "cfgdir": "/etc/taos",
    "host": "127.0.0.1",
    "port": 6030,
    "user": "root",
    "password": "taosdata",
    "databases": "db",
    "confirm_parameter_prompt": "no",
    "specified_table_query":
      {
       "concurrent":1,
       "mode":"sync",
       "interval":0,
       "restart":"yes",
       "keepProgress":"yes",
       "sqls": [
        {
          "sql": "select * from stb00_0 ;",
          "result": "./subscribe_res0.txt"
        }]
      },
    "super_table_query":
      {
       "stblname": "stb0",
       "threads":1,
       "mode":"sync",
       "interval":10000,
       "restart":"yes",
       "keepProgress":"yes",
       "sqls": [
        {
          "sql": "select * from xxxx where ts > '2021-02-25 11:35:00.000' ;",
          "result": "./subscribe_res1.txt"
        }]
      }
}
```

"filetype": 本taosBenchmark实例进行哪种功能测试。"subscribe"表示数据查询功能。必选项。**

"cfgdir": 配置文件taos.cfg所在的路径。因为taosBenchmark通过包含taos的动态库，去链接taosd服务，所以需要做好配置文件。可选项，缺省是 "/etc/taos"路径。

"host": taosd服务的FQDN。可选项，缺省是“localhost“。

"port": taosd服务的端口号。可选项，缺省是6030。

"user": 用户名。可选项，缺省是“root“。

"password": 密码。可选项，缺省是“taosdata"。

"databases": 数据库名称。必选项。**

"confirm_parameter_prompt": 执行过程中提示是否确认，为no时，执行过程无需手工输入enter。可选项，缺省是no。

注意：这里的订阅查询sql目前只支持select * ，其余不支持。

"specified_table_query": 指定表的订阅。

"concurrent": 并发执行sqls的线程数，可选项，缺省是1。每个线程都执行所有的sqls。

"mode": 订阅模式。目前支持同步和异步订阅，缺省是sync。

"interval": 执行订阅的间隔，单位是秒。可选项，缺省是0。

"restart": 订阅重启。"yes"：如果订阅已经存在，重新开始，"no": 继续之前的订阅。(请注意执行用户需要对 dataDir 目录有读写权限)

"keepProgress": 保留订阅信息进度。yes表示保留订阅信息，no表示不保留。该值为yes，restart为no时，才能继续之前的订阅。

"resubAfterConsume":  配合 keepProgress 使用，在订阅消费了相应次数后调用 unsubscribe 取消订阅并再次订阅。

"sql": 查询语句。必选项。

"result": 查询结果写入的文件名。可选项，缺省是空，表示查询结果不写入文件。

"super_table_query": 对超级表中所有子表的订阅。

"stblname": 超级表名称。必选项。

"threads": 并发执行sqls的线程数，可选项，缺省是1。每个线程都执行所有的sqls。

"mode": 订阅模式。

"interval": 执行sqls的间隔，单位是秒。可选项，缺省是0。

"restart": 订阅重启。"yes"：如果订阅已经存在，重新开始，"no": 继续之前的订阅。

"keepProgress": 保留订阅信息进度。yes表示保留订阅信息，no表示不保留。该值为yes，restart为no时，才能继续之前的订阅。

"resubAfterConsume":  配合 keepProgress 使用，在订阅消费了相应次数后调用 unsubscribe 取消订阅并再次订阅。

"sql": " select count(*) from xxxx "。查询语句，其中表名必须写成 “xxxx”，实例会自动替换成子表名。

"result": 查询结果写入的文件名。可选项，缺省是空，表示查询结果不写入文件。 注意：每条sql语句后的保存结果的文件不能重名，且生成结果文件时，文件名会附加线程号。
