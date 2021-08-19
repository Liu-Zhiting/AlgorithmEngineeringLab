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

### triangle-counting

设 $$G=(V,E)$$ 是一个简单的无向图。我们定义三角形是三个顶点的集合 $$v1, v2,v3 \in V $$使得无向边 $$(v1,v2)$$、$$(v2, v3)$$ 和 $$(v1,v3)$$ 出现在 E 中。三角形计数问题将无向图$$G$$作为输入数据并返回G中的三角形数量。

### rank-and-count 算法



**rank**



**count**



## 任务1：merge实现的triangle-counting



## 任务2：hash实现的triangle-counting



## 任务3：矩阵乘法实现的triangle-counting





**参考资料**

