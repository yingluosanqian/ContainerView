# ContainerView: A Handy C++ Library for Displaying STL Containers

## 1. 介绍

这是一个 C++ 库，重载了部分 STL 容器的 `<<` 操作符，实现对这部分 C++ STL 容器的打印。

参考了 [这里](https://github.com/adah1972/output_container)，目前实现了原项目的所有功能，并且后续会做出更多的更新，支持更多的容器打印。

### 1.1 目前支持的容器

1. 所有含有 `begin()` 和 `end()` 的容器。包括但不限于 `vector`，`map`，`set`。
2. `std::pair`

## 2. 使用

直接引用 `container_view.h` 即可。

### 2.1 一个简单的示例

``` c++
#include "container_view.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<int>> vec_int{
            {0, 1, 2},
            {0, 1},
            {0},
    };
    std::cout << vec_int << std::endl;

    return 0;
}
```

输出结果：

``` text
[[0, 1, 2], [0, 1], [0]]
```

### 2.2  更多的示例

``` c++
// std::array
std::array<int, 5> ar{1, 2, 3, 4, 5};
std::cout << ar << std::endl;
// [1, 2, 3, 4, 5]
    
// std::set
std::set<int> st{1, 2, 3};
std::cout << st << std::endl;
// [1, 2, 3]

// std::map
std::map<int, std::string> mp{
        {0, "zero"},
        {1, "one"},
        {2, "two"},
        {3, "three"},
};
std::cout << mp << std::endl;
// [0: zero, 1: one, 2: two, 3: three]

// std::pair
std::pair<std::vector<int>, std::string> pr{
        {0, 1, 2},
        {"zero-one-two"}
};
std::cout << pr << std::endl;
// <[0, 1, 2], zero-one-two>
```
