# 4-bfs

对图进行广度优先搜索

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

最基础的BFS就是自顶向下的，它只维护一个队列，每迭代一次同时进行出队和入队。这里提供一个维护两个队列的思路，便于将BFS并行化：

1. 第一次迭代前将根节点推入队列1中，并标记为已访问
2. 每次迭代开始时，将队列1中的每个节点的未访问过的邻居推入队列2，并将它们标记为已访问
3. 每次迭代结束前，交换队列1与队列2

**任务要求**

参考以上内容，查阅相关资料，编写top_down.cpp文件中的top_down函数以实现自顶向下的BFS算法，并且编译运行，程序会判断结果是否正确并测量运行时间



## 任务2：自底向上的BFS

自顶向下实现的广度优先搜索在某些情况下存在性能问题。 在这些情况下，广度优先搜索步骤的替代实现可能更有效。 代替遍历边界中的所有顶点并标记与边界相邻的所有顶点，可以通过让每个顶点检查它是否应该添加到边界来实现 BFS！

该算法有时被称为 BFS 的“自底向上”实现，因为每个顶点都“向上查找 BFS 树”以找到其祖先。

**任务要求**

参考以上内容，查阅相关资料，编写buttom_up.cpp文件中的buttom_up函数以实现自底向上的BFS算法，并且编译运行，程序会判断结果是否正确并测量运行时间



## 任务3：混合的BFS

在 BFS 的某些步骤中，“自底向上”BFS 明显比自顶向下版本快。 在其他步骤中，自顶向下版本明显更快。 这表明您的实现有重大的性能改进，如果 __您可以根据边界的大小或图的其他属性在“自上而下”和“自下而上”公式之间动态选择！__ 

**任务要求**

参考以上内容，查阅相关资料，编写hybrid.cpp文件中的hybrid函数以实现混合的BFS算法，并且编译运行，程序会判断结果是否正确并测量运行时间



## 参考资料

http://www.scottbeamer.net/pubs/beamer-sc2012.pdf

https://github.com/stanford-cs149/asst4