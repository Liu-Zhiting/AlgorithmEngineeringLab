# 3-page-rank

计算简单图的page-rank值

## 基本信息

**输入文件格式**

邻接表。

文件为二进制格式，最前端4个字节是一个无符号32位整数n，表示图的点数，之后n个4字节无符号整数表示n个点的出度out_degree，之后每out_degree[i]个4字节无符号整数表示被第i个点指向的点

**函数格式**

```c++
void example_function(const AdjointList &graph, Solution &solution)
```

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
page-rank <filename>
```

## 数据结构

### struct Node


```c++
typedef struct Node

{

  uint32_t value;

  struct Node *next;

} Node;
```

`Node`结构用于`class AdjointList`中链表的实现

### class AdjointList

邻接表数据

```c++
uint32_t vertex_count;
uint32_t edge_count;
uint32_t *out_degree; // out_degree[i]储存节点i的出度
Node *vertex;   // vertex[i]的next指向一个包含节点i指向的点的链表
```

构造函数与析构函数

```c++
AdjointList();
AdjointList(const int vertex_count);
AdjointList(const AdjointList &other); // 拷贝构造函数
~AdjointList();
```

常用运算符与函数

```c++
bool operator==(const AdjointList &other) const;
AdjointList &operator=(const AdjointList &other);

bool add_edge(const int from, const int to);
bool search_edge(const int from, const int to) const;
bool remove_edge(const int from, const int to);

uint32_t **convert_to_adjoint_matrix() const; // 转换为图的邻接矩阵
void dump_adjoint_list() const; // 将表内数据输出在控制台中
```

完整内容参见adjoint_list.hpp与adjoint_list.cpp

### class Solution

数据

```c++
uint32_t size;  // 图的节点个数
double *value;  // value[i]存储最终的第i个节点的PageRank值
```

构造函数与析构函数

```c++
Solution();
Solution(uint32_t size);
Solution(const Solution &other);  // 拷贝构造函数
~Solution();
```

常用运算符与函数

```c++
bool operator==(const Solution &other) const;
Solution &operator=(const Solution &other);

void clear();
void dump() const; // 将数据输出在控制台中
```

完整内容参见solution.hpp与solution.cpp



## 预备知识：PageRank

PageRank是[Google](https://zh.wikipedia.org/wiki/Google)公司所使用的对其搜索引擎搜索结果中的网页进行排名的一种算法，它本质上是一种以网页之间的超链接个数和质量作为主要因素粗略地分析网页的重要性的算法，其基本假设是：更重要的页面往往更多地被其他页面引用（或称其他页面中会更多地加入通向该页面的超链接）。

PageRank的结果来源于一种基于图论的数学算法。它将万维网上所有的网页视作节点（node），而将超链接视作边（edge）。通向该网页的超链接称做“对该网页的投票（a vote of support）”。

假设一个由4个网页组成的集合：**A**，**B**，**C**和**D**。这里将同一页面中多个指向相同的链接视为同一个链接。对于特定的页面，它的**PR值**（PageRank值）将均分到该页面所链接的页面上。

>注：此处以及下文中的链接均为单向链接，即A->B不等价于B->A

如果所有页面都只链接至**A**，那么**A**的PR值将是**B**，**C**及**D**的PR值之和，即：

<img src="https://latex.codecogs.com/svg.image?PR(A)=PR(B)&plus;PR(C)&plus;PR(D)" title="PR(A)=PR(B)+PR(C)+PR(D)" />

重新假设**B**链接到**A**和**C**，**C**链接到**A**，并且**D**链接到**A**,**B**,**C**。最初一个页面总共只有一票。所以**B**给**A** ,**C**每个页面半票。以此类推，**D**投出的票只有三分之一加到了**A**的PR值上：

<img src="https://latex.codecogs.com/svg.image?PR(A)=\frac{PR(B)}{2}&plus;\frac{PR(C)}{1}&plus;\frac{PR(D)}{3}" title="PR(A)=\frac{PR(B)}{2}+\frac{PR(C)}{1}+\frac{PR(D)}{3}" />

换句话说，算法将根据每个页面连出总数L(x)（**亦即图中节点的出度**）平分该页面的PR值，并将其加到其所指向的页面：

<img src="https://latex.codecogs.com/svg.image?PR(A)=\frac{PR(B)}{L(B)}&plus;\frac{PR(C)}{L(C)}&plus;\frac{PR(D)}{L(D)}" title="PR(A)=\frac{PR(B)}{L(B)}+\frac{PR(C)}{L(C)}+\frac{PR(D)}{L(D)}" />

最后，所有这些PR值被换算成百分比形式再乘上一个修正系数 d。d的大小可以由一般上网者使用浏览器书签功能的频率的平均值估算得到，其意义是：任意时刻，用户访问到某页面后继续访问下一个页面的概率。**一般情况下，修正系数d的取值是0.85**。由于“没有向外链接的网页”传递出去的PR值会是0，而这会递归地导致指向它的页面的PR值的计算结果同样为零，所以赋给每个页面一个最小值(1-d)/N:

<img src="https://latex.codecogs.com/svg.image?PR(A)=\begin{pmatrix}&space;&space;\frac{PR(B)}{L(B)}&plus;\frac{PR(C)}{L(C)}&plus;\frac{PR(D)}{L(D)}\end{pmatrix}d&plus;\frac{1-d}{N}" title="PR(A)=\begin{pmatrix} \frac{PR(B)}{L(B)}+\frac{PR(C)}{L(C)}+\frac{PR(D)}{L(D)}\end{pmatrix}d+\frac{1-d}{N}" />

所以，对于某个页面i，其对应PR值大小的计算公式如下：

<img src="https://latex.codecogs.com/svg.image?PR(p_{i})=\frac{1-d}{N}&plus;d\sum_{p_{j}\in&space;Q(p_{i})}\frac{PR(p_{j})}{L(p_{j})}" title="PR(p_{i})=\frac{1-d}{N}+d\sum_{p_{j}\in Q(p_{i})}\frac{PR(p_{j})}{L(p_{j})}" />

其中Q(pi)是链入页面pi的集合。

集合中所有页面的PR值可以由一个特殊的邻接矩阵的特征向量R表示。下面写出R的迭代式：

<img src="https://latex.codecogs.com/svg.image?Set\&space;matrix\&space;M=\begin{bmatrix}&space;&space;&space;l(p_{1},p_{1})&space;&&space;l(p_{1},p_{2})&space;&&space;\cdots&space;&&space;l(p_{1},p_{N})&space;\\&space;&space;&space;l(p_{2},p_{1})&space;&&space;\ddots&space;&&space;&&space;\vdots&space;\\&space;&space;&space;\vdots&space;&&space;&&space;l(p_{i},p_{j})&space;&&space;\\&space;&space;&space;l(p_{N},p_{1})&space;&&space;&&space;&l(p_{N},p_{N})\end{bmatrix}&space;\\\\Where\&space;l(p_{i},p_{j})&space;=\left\{\begin{matrix}\frac{1}{L(j)},&space;page\&space;j\&space;has\&space;a\&space;link\&space;to\&space;page\&space;i\&space;\\0,&space;page\&space;j\&space;has\&space;no\&space;link\&space;to\&space;page\&space;i&space;\\\end{matrix}\right.,&space;\\L(j)\&space;is\&space;the\&space;total\&space;number\&space;of\&space;links\&space;out\&space;of\&space;page\&space;j&space;(that\&space;is,\&space;the\&space;outgoing\&space;degree\&space;of\&space;nodes\&space;in\&space;the\&space;graph)\\Then\&space;R'=&space;\begin{bmatrix}PR(p_{1})&space;\\PR(p_{2})&space;\\\vdots&space;\\PR(p_{N})&space;\\\end{bmatrix}&space;=&space;\frac{1-d}{n}1&plus;dMR" title="Set\ matrix\ M=\begin{bmatrix} l(p_{1},p_{1}) & l(p_{1},p_{2}) & \cdots & l(p_{1},p_{N}) \\ l(p_{2},p_{1}) & \ddots & & \vdots \\ \vdots & & l(p_{i},p_{j}) & \\ l(p_{N},p_{1}) & & &l(p_{N},p_{N})\end{bmatrix} \\\\Where\ l(p_{i},p_{j}) =\left\{\begin{matrix}\frac{1}{L(j)}, page\ j\ has\ a\ link\ to\ page\ i\ \\0, page\ j\ has\ no\ link\ to\ page\ i \\\end{matrix}\right., \\L(j)\ is\ the\ total\ number\ of\ links\ out\ of\ page\ j (that\ is,\ the\ outgoing\ degree\ of\ nodes\ in\ the\ graph)\\Then\ R'= \begin{bmatrix}PR(p_{1}) \\PR(p_{2}) \\\vdots \\PR(p_{N}) \\\end{bmatrix} = \frac{1-d}{n}1+dMR" />

容易证明，上述迭代式中R是收敛的。



## 任务1：迭代算法

将每个点的初始PR值均设置为1/N。设定一个精确度ε，迭代计算

<img src="https://latex.codecogs.com/svg.image?R=\frac{1-d}{n}1&plus;dMR" title="R=\frac{1-d}{n}1+dMR" />

直到|R’-R|≤ε 即可。实践中，几次迭代后R'即可在极高的精确度下估计R的值。

**任务要求**

参考以上内容，查阅相关资料，编写iterate_rank.cpp文件中的iterate_rank函数以实现PageRank的迭代算法，并且编译运行，程序会判断结果是否正确并测量运行时间



## 任务2：代数算法

将

<img src="https://latex.codecogs.com/svg.image?R=\frac{1-d}{n}1&plus;dMR" title="R=\frac{1-d}{n}1+dMR" />

变换得到：

<img src="https://latex.codecogs.com/svg.image?(I-dM)R=\frac{1-d}{n}1" title="(I-dM)R=\frac{1-d}{n}1" />

<img src="https://latex.codecogs.com/svg.image?R=(I-dM)^{-1}\frac{1-d}{n}1" title="R=(I-dM)^{-1}\frac{1-d}{n}1" />

因此，可以通过矩阵求逆与矩阵乘法来直接计算所有网页的PR值。

**任务要求**

参考以上内容，查阅相关资料，编写accurate_rank.cpp文件中的accurate_rank函数以实现PageRank的代数算法，并且编译运行，程序会判断结果是否正确并测量运行时间



## 参考文献

\[1\]MLPod. PageRank算法详解[EB/OL]. [2021-10-15]. https://zhuanlan.zhihu.com/p/137561088.

\[2\]维基百科. PageRank[EB/OL]. [2021-10-15]. https://zh.wikipedia.org/wiki/PageRank.