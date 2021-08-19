# 4-bfs

对图进行广度优先搜索

### 基本信息

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
bfs <filename>
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
uint32_t *distance;  // distance[i]存储第i个节点与根节点的距离
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

## 任务1：自顶向下的BFS



## 任务2：自底向上的BFS



## 任务3：混合的BFS

