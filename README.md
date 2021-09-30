# 算法工程实验

## 环境安装与配置

实验代码需要在linux环境下安装与编译，推荐Windows用户使用WSL或者虚拟机，安装说明如下：

- [Windows Subsystem for Linux(WSL)](https://docs.microsoft.com/zh-cn/windows/wsl/install-win10)
- [VMWare WorkStaion Player](https://www.vmware.com/cn/products/workstation-player.html)

实验代码中使用了并行工具Cilk。[Cilk](https://en.wikipedia.org/wiki/Cilk)是由MIT 的 [Charles E. Leiserson](https://people.csail.mit.edu/cel/) 及其团队最初开发的并行工具，在gcc5(g++5)中被加入，gcc8(g++8)中被弃用，此后版本的gcc（与g++）不再支持Cilk。Cilk后来被 Cilk Arts 公司商业化为 Cilk++。该公司随后被英特尔收购，因特尔将其称为 Cilk Plus。此外，Leiserson 团队发布了Cilk的开源版本[OpenCilk](https://cilk.mit.edu/)，这是一个修改过的clang10编译器，支持Cilk语法。

完成本实验时建议使用gcc7(g++7)编译并运行，或者安装OpenCilk并使用clang10(clang++10)。

## 实验内容

### 排序

对一维整数数组进行排序

### 矩阵乘法

将两个相同大小的整数方阵相乘

### Page Rank

计算简单图的page-rank值

### BFS

对图进行广度优先搜索

### Triangle Counting

计算无向图中的三角形（三元环）的个数

## 数据格式

### array

长度为n的一维整数数组。

文件为二进制格式，最前端4个字节是一个无符号32位整数n，表示数组的长度，之后的n个4字节有符号整数为数据内容

### matrix

大小为n*n的二维整数数组。

文件为二进制格式，最前端4个字节是一个无符号32位整数n，表示矩阵的边长，之后的n*n个4字节有符号整数为数据内容

### adjoint_list

邻接表。

文件为二进制格式，最前端4个字节是一个无符号32位整数n，表示图的点数，之后n个4字节无符号整数表示n个点的出度out_degree，之后每out_degree[i]个4字节无符号整数表示被第i个点指向的点

