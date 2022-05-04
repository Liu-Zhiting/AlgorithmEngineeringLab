# 6-tsp-validator

tsp解的验证器

## 基本信息

**输入文件格式**

1. tsplib格式的地图文件

2. tsp路径上的点的序号，存储在文本文件中，用空格或回车分隔

**编译指令**

```makefile
make
```

**清理指令**

```makefile
make clean
```

**运行指令**

```bash
tspvd <dataset-name> <answer-file-name>
```

## 示例

输入

```bash
./tspvd
```

输出

```
Usage: tspvd <dataset-name> <answer-file-name>
Available DataSet:
ch71009
```

输入

```bash
./tspvd ch71009 ./answer.txt
```

输出

```
============================================================
===========      Algorithm Engineering Lab      ============
============================================================
Project 6: TSP Validator
------------------------------------------------------------
Loading data . . .
Load succeeded!
Data Info: 
Name: ch71009
Node Count: 71009
------------------------------------------------------------
Your answer builds a tour of 300096893.22
The optimized distance is 4566563.00
Your answer gets 295530330.22 longer distance than optimized
Your answer is approximately 1.52% of optimized
============================================================
```


## 参考

https://www.math.uwaterloo.ca/tsp/concorde.html

http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/
