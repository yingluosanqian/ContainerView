# ContainerView: A Handy C++ Library for Displaying STL Containers

## 1. 概述

`container_view` 库提供了通过重载 `<<` 操作符来输出 STL 容器以及对象数据成员的功能。这使得可以直接将各种对象包括但不限于（`std::pair`、`std::tuple`、结构体对象或类对象）直接输出到标准输出流（如 `std::cout`）。

## 2. 快速开始

需要支持 C++20 的编译器，只需要引用头文件 `container_view.h` 即可（在项目 `include` 目录下）。

``` c++
#include "container_view.h"
#include <iostream>
#include <vector>
#include <map>

int main() {
    std::vector<int> vec = {1, 2, 3, 4};
    std::cout << "Vector: " << vec << std::endl;
    // Vector: [1, 2, 3, 4]

    std::map<std::string, int> map = {{"one", 1}, {"two", 2}};
    std::cout << "Map: " << map << std::endl;
    // Map: [one: 1, two: 2]

    std::pair<int, double> myPair = {1, 2.3};
    std::cout << "Pair: " << myPair << std::endl;
    // Pair: (1, 2.3)
    
    struct Point {
        int x, y;
        double value;
    } point(0, 0, -1);
    std::cout << "Point: " << point << std::endl;
    // Point: (0, 0, -1)

    return 0;
}
```

### 2.1 示例一

考虑如下场景：

* 给定长度为 $n$ 的字符串 $S$，保证仅仅包含大小写字母，统计 $S$ 中出现的所有字母的所有出现位置。

很容易利用 `std::map` 和 `std::vector` 写出如下代码:

``` c++
std::string s = "AAbcAAbbA";
std::map<char, std::vector<int>> pos;
for (auto i = 0; i < s.size(); i++) {
    pos[s[i]].push_back(i);
}
```

那么写完之后，如果想查看 pos 的结果，要怎么做呢？传统的方法大概是这样的：

``` c++
for (auto &[ch, vec] : pos) {
    std::cout << ch << ": ";
    for (auto e : vec) std::cout << e << ' ';
    std::cout << std::endl;
}
```

然而，利用 `container_view`，只需要一行代码：

``` c++
std::cout << pos << std::endl;
```

测试效果：

``` text
{A: [0, 1, 4, 5, 8], b: [2, 6, 7], c: [3]}
```

### 2.2 示例二

考虑如下场景：

* 给定一个长度为 $n$ 的整数数组（数组元素互不相同），对于 $i=1,3,5,...$，求其前 $i$ 项的中位数，并数字该数字在数组中的下标；
* $n\le 10^5$。

很容易利用对顶堆写出如下代码：

``` c++
struct Data {
    int value, pos;
    bool operator < (const Data &other) const { return value < other.value; }
    bool operator > (const Data &other) const { return value > other.value; }
};

std::vector<std::pair<int, int>> solve(std::vector<int> ar) {
    std::vector<std::pair<int, int>> result;
    // 小顶堆，存放最大的若干个数字
    std::priority_queue<Data> small;
    // 大顶堆，存放最小的若干个数字
    std::priority_queue<Data, std::vector<Data>, std::greater<>> big;
    for (auto i = 0; i < ar.size(); i++) {
        big.push({ar[i], i + 1});
        if (i % 2 == 0) {
            // 分摊两个堆中的元素数量
            while (small.size() < big.size()) {
                small.push(big.top());
                big.pop();
            }
            result.emplace_back(small.top().value, small.top().pos);
        }
    }
    return result;
}
```

然而，如果我们想要跟踪这份代码每一次迭代后，两个堆中的元素情况，这将会非常麻烦。比如我们至少要写出这样的屎山：

``` c++
std::cout << "After " << i << "-th iteration:\n";
auto small_copy = small;
std::cout << "[";
while (!small_copy.empty()) {
    std::cout << "(" << small_copy.top().value << ", " << small_copy.top().pos << "), ";
    small_copy.pop();
}
std::cout << "]" << std::endl;
auto big_copy = small;
std::cout << "[";
while (!big_copy.empty()) {
    std::cout << "(" << big_copy.top().value << ", " << big_copy.top().pos << "), ";
    big_copy.pop();
}
std::cout << "]" << std::endl;
```

因为一方面堆是不可遍历的（遍历堆意味着要将堆中元素 pop 出去，破坏了堆的结果），另一方面 Data 类型的对象也无法直接打印至控制台。

然而，利用 `container_view` 可以轻松且自然地解决这个问题，只需要三行即可，并且非常符合使用习惯：

``` c++
std::cout << "After " << i << "-th iteration:\n";
std::cout << "small: " << small << std::endl;
std::cout << "big: " << big << std::endl << std::endl;
```

测试效果：

``` text
输入：
1, 3, 5, 7, 9, 11, 6

输出：
After 0-th iteration:
small: [(1, 1)]
big: []

After 1-th iteration:
small: [(1, 1)]
big: [(3, 2)]

After 2-th iteration:
small: [(3, 2), (1, 1)]
big: [(5, 3)]

After 3-th iteration:
small: [(3, 2), (1, 1)]
big: [(5, 3), (7, 4)]

After 4-th iteration:
small: [(5, 3), (3, 2), (1, 1)]
big: [(7, 4), (9, 5)]

After 5-th iteration:
small: [(5, 3), (3, 2), (1, 1)]
big: [(7, 4), (9, 5), (11, 6)]

After 6-th iteration:
small: [(6, 7), (5, 3), (3, 2), (1, 1)]
big: [(7, 4), (9, 5), (11, 6)]
```

如果想查看结果，也非常方便，只需要：

``` c++
std::cout << "Result: " << result << std::endl;
// Result: [(1, 1), (3, 2), (5, 3), (6, 7)]
```

## 3. 使用

* 需要支持 C++20 及以上的编译器。
* 将项目的 `include` 目录下的文件 `container_view.h` 拷贝至本地，直接引用头文件即可。
* 对于想要打印的对象 `x`，直接 `std::cout << x` 即可。

### 3.1 对 STL 容器的打印效果展示

``` c++
#include "container_view.h"
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <tuple>
#include <forward_list>

int main() {
    // Pair
    std::pair<int, double> p = {1, 3.14};
    std::cout << "Pair: " << p << std::endl;

    // Tuple
    std::tuple<int, char, double> t = {1, 'a', 2.3};
    std::cout << "Tuple: " << t << std::endl;

    // Array
    std::array<int, 3> arr = {1, 2, 3};
    std::cout << "Array: " << arr << std::endl;

    // Deque
    std::deque<double> dq = {1.1, 2.2, 3.3};
    std::cout << "Deque: " << dq << std::endl;

    // Forward List
    std::forward_list<int> fl = {1, 2, 3};
    std::cout << "Forward List: " << fl << std::endl;

    // Initializer list (note: we cannot print directly, used only for example)
    std::initializer_list<int> il = {1, 2, 3};

    // List
    std::list<int> lst = {1, 2, 3};
    std::cout << "List: " << lst << std::endl;

    // Vector
    std::vector<std::string> vec = {"hello", "world"};
    std::cout << "Vector: " << vec << std::endl;

    // Set
    std::set<std::string> set = {"apple", "banana", "cherry"};
    std::cout << "Set: " << set << std::endl;

    // Multiset
    std::multiset<int> mset = {4, 5, 4, 1};
    std::cout << "Multiset: " << mset << std::endl;

    // Unordered Set
    std::unordered_set<int> uset = {4, 5, 6, 7};
    std::cout << "Unordered Set: " << uset << std::endl;

    // Unordered Multiset
    std::unordered_multiset<int> umset = {4, 5, 6, 6};
    std::cout << "Unordered Multiset: " << umset << std::endl;

    // Map
    std::map<char, int> map = {{'a', 1}, {'b', 2}};
    std::cout << "Map: " << map << std::endl;

    // Multimap
    std::multimap<char, int> mmap = {{'a', 1}, {'a', 2}};
    std::cout << "Multimap: " << mmap << std::endl;

    // Unordered Map
    std::unordered_map<char, int> umap = {{'c', 3}, {'b', 2}};
    std::cout << "Unordered Map: " << umap << std::endl;

    // Unordered Multimap
    std::unordered_multimap<char, int> ummap = {{'c', 3}, {'c', 4}};
    std::cout << "Unordered Multimap: " << ummap << std::endl;

    return 0;
}
/*
Pair: (1, 3.14)
Tuple: (1, a, 2.3)
Array: [1, 2, 3]
Deque: [1.1, 2.2, 3.3]
Forward List: [1, 2, 3]
List: [1, 2, 3]
Vector: [hello, world]
Set: [apple, banana, cherry]
Multiset: [1, 4, 4, 5]
Unordered Set: [7, 6, 5, 4]
Unordered Multiset: [6, 6, 5, 4]
Map: {a: 1, b: 2}
Multimap: {a: 1, a: 2}
Unordered Map: {b: 2, c: 3}
Unordered Multimap: {c: 4, c: 3}
*/
```

### 3.2 对 STL 容器嵌套的打印效果展示


1. **嵌套的 Vector 和 Map**
    - 一个 `vector` 中包含 `map`，`map` 的值是 `list`。

```cpp
std::vector<std::map<std::string, std::list<int>>> complexVector = {
    {{"key1", {1, 2, 3}}, {"key2", {4, 5}}},
    {{"key3", {6}}, {"key4", {7, 8, 9}}}
};
std::cout << "Complex Vector: " << complexVector << std::endl;
// Complex Vector: [{key1: [1, 2, 3], key2: [4, 5]}, {key3: [6], key4: [7, 8, 9]}]
```

2. **嵌套的 Tuple**
    - 一个 `tuple` 包含 `set` 和 `vector`，以及一个 `pair`。

```cpp
std::tuple<std::set<int>, std::vector<std::string>, std::pair<std::string, double>> complexTuple = {
    {1, 2, 3},
    {"apple", "banana"},
    {"pi", 3.14159}
};
std::cout << "Complex Tuple: " << complexTuple << std::endl;
// Complex Tuple: ([1, 2, 3], [apple, banana], (pi, 3.14159))
```

3. **Map 的键和值中嵌套容器**
    - `map` 的键是 `vector`，值是 `unordered_set`。

```cpp
std::map<std::vector<int>, std::unordered_set<std::string>> complexMap = {
    {{1, 2}, {"hello", "world"}},
    {{3, 4}, {"foo", "bar", "baz"}}
};
std::cout << "Complex Map: " << complexMap << std::endl;
// Complex Map: {[1, 2]: [world, hello], [3, 4]: [baz, bar, foo]}
```

4. **Multimap 包含 Tuple**
    - `multimap` 的值是 `tuple`，包含 `string` 和 `deque`.

```cpp
std::multimap<int, std::tuple<std::string, std::deque<double>>> complexMultimap = {
    {1, {"test", {1.1, 2.2, 3.3}}},
    {2, {"demo", {4.4, 5.5}}}
};
std::cout << "Complex Multimap: " << complexMultimap << std::endl;
// Complex Multimap: {1: (test, [1.1, 2.2, 3.3]), 2: (demo, [4.4, 5.5])}
```

5. **嵌套的 Unordered Map**
    - `unordered_map` 嵌套 `unordered_map`。

```cpp
std::unordered_map<std::string, std::unordered_map<int, std::string>> nestedUnorderedMap = {
    {"first", {{1, "one"}, {2, "two"}}},
    {"second", {{3, "three"}}}
};
std::cout << "Nested Unordered Map: " << nestedUnorderedMap << std::endl;
// Nested Unordered Map: {second: {3: three}, first: {2: two, 1: one}}
```

### 3.3 更复杂的嵌套：结构体和容器嵌套

``` c++
struct Person {
    std::string name;
    int age;
};

struct Project {
    std::string projectName;
    std::list<Person> members;
};

struct Department {
    std::string leader;
    std::vector<Person> staff;
};

int main() {
    std::vector<Person> team = {
            {"Alice", 30},
            {"Bob", 25},
            {"Charlie", 35}
    };
    std::cout << "Team: " << team << std::endl;

    Project project = {
            "Deep Space",
            {
                    {"Dave", 40},
                    {"Eve", 29}
            }
    };
    std::cout << project << std::endl;

    std::map<std::string, Department> company = {
            {"HR", {"Alice", {{"Bob", 28}, {"Charlie", 34}}}},
            {"IT", {"Dave", {{"Eve", 24}, {"Frank", 30}}}}
    };
    std::cout << "Company Departments: " << company << std::endl;

    return 0;
}
/*
Team: [(Alice, 30), (Bob, 25), (Charlie, 35)]
(Deep Space, [(Dave, 40), (Eve, 29)])
Company Departments: {HR: (Alice, [(Bob, 28), (Charlie, 34)]), IT: (Dave, [(Eve, 24), (Frank, 30)])}
*/
```

## 4. 其他

以后可能会根据更多的需求进行相应的更新。

如有建议欢迎联系：[Email me](mailto:yingluosanqian@gmail.com)

