# 5-triangle-counting

计算无向图中的三角形（三元环）的个数

## 基本信息

**输入文件格式**

邻接表。

文件为二进制格式，最前端4个字节是一个无符号32位整数n，表示图的点数，之后n个4字节无符号整数表示n个点的出度out_degree，之后每out_degree[i]个4字节无符号整数表示被第i个点指向的点

**函数格式**

```c++
uint32_t example_function(const Graph &graph)
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
triangle-counting <filename>
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

## 预备知识

### triangle-counting 定义

设 G=(V,E) 是一个简单的无向图。我们定义三角形是三个顶点的集合v1, v2,v3 ∈ V ，使得无向边 (v1,v2)、(v2, v3) 和 (v1,v3) 出现在 E 中。三角形计数问题将无向图G作为输入数据并返回G中的三角形数量。



## 任务1：rank-and-count算法

### rank

rank步骤构造一个rank数组R，R[i]为标号为i的节点的rank值，然后根据rank数组将无向图转换为有向图。

假定无向图中所有节点的度数都存储在数组D中，数组的下标为节点的ID，D[i]为标号为i的节点的度数。步骤如下：

1. 构造rank数组。将D中的节点度数与节点ID绑定到一起存入新数组中，按节点度数对它升序排序。构造相同长度的数组R来存储节点在新数组中排序后的下标（rank值）。对于R[i] < R[j]，总有D[i] ≤ D[j]。
2. 将无向图转换为有向图。遍历每条边，只保留从rank值较小的节点指向rank值较大的节点的单向边。

步骤完成后得到一个有向图，记为A+。

**实例**

我们用一个简单的实例说明一下。如图，D数组为{2,4,1,3,2}，构建一个结构体数组

```c++
{
    {0,2},{1,4},{2,1},{3,3},{4,2}
}
```

排序后为

```c++
{
    {2,1},{0,2},{4,2},{3,3},{1,4}
}
```

那么R数组的值为

```c++
R[2] = 0;
R[0] = 1;
R[4] = 2;
R[3] = 3;
R[1] = 4;
```

之后遍历每条边，将它们转换为从rank值较小的点指向rank值较大的点的有向边。

### count

count步骤利用rank步骤得到的有向图A+来数三角形个数。步骤如下：

1. 申请一个数组C，大小是邻接表A+中所有点的邻居数量总和
2. 对A+中每个点v，以及v的每个邻居节点w，求A+[v]与A+[w]的**交集**I
3. 将I的元素个数存储在C[ρ(v,w)]中，其中ρ(v,w)是对每一对v,w返回值均不相同的函数，ρ(v,w)为每一对v,w提供唯一的下标
4. C中的所有数值求和即为三角形个数

关于求A+[v]与A+[w]的**交集**，这里提供两种思路：

**merge实现求两数组交集**

1. 首先对两个数组排序
2. 比较当前指向的两个元素，如果不相等，值小的指针后移一位
3. 如果相等，将结果添加到解集中，然后两个指针都后移一位
4. 当任意一个指针超出尾部后，结束

**hash实现求两数组交集**

1. 遍历数组1，将所有值加入哈希表
2. 遍历数组2，在哈希表中查询，如查询到该元素，则将其添加至解集

### 任务要求

参考以上内容，查阅相关资料，编写tc_merge.cpp文件中的tc_merge函数、tc_hash.cpp文件中的tc_hash函数，以实现rank-and-count算法，并且编译运行，程序会判断结果是否正确并测量运行时间

## 任务2：矩阵乘法实现的triangle-counting

将图表示为邻接矩阵A，A\[i\]\[j\] ∈ {0,1}。A\[i\]\[j\] = 1时表示点i与j连通，反之为不连通。

由矩阵乘法的定义，矩阵A^2的含义是：

<img src="https://latex.codecogs.com/svg.image?(A^{2})_{ij}=\sum_{k=1}^{n}a_{ik}&space;\cdot&space;a_{kj}" title="(A^{2})_{ij}=\sum_{k=1}^{n}a_{ik} \cdot a_{kj}" />

A\[i\]\[k\] · A\[k\]\[j\]=1的含义是，点i到点k是连通的且点k到点j是连通的，这也就是说，A^2\[i\]\[j\]的值表示，点i到某个点k再到点j的连通路径的数量。

那么，对两个点i,j，A\[i\]\[j\] · A^2\[i\]\[j\] 的含义就是包含点i,j的三角形的个数。我们可以用2层循环求出每一对点所在的三角形计数C。在这个过程中，每个三角形被数了三次，所以最终的结果应该是 C/3。

**任务要求**

参考以上内容，查阅相关资料，编写tc_matrix.cpp文件中的tc_matrix函数以实现矩阵乘法实现的triangle-counting算法，并且编译运行，程序会判断结果是否正确并测量运行时间



## 参考文献

\[1\]Shun J, Tangwongsan K. Multicore triangle computations without tuning[C]//2015 IEEE 31st International Conference on Data Engineering. IEEE, 2015: 149-160.

\[2\]www.cs.cmu.edu. Triangle Counting and Matrix Multiplication[EB/OL]. [2021-10-15]. https://www.cs.cmu.edu/~15750/notes/lec1.pdf.

