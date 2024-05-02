#include "../include/container_view.h"
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
