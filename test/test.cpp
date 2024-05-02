#include "../include/container_view.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <array>

int main() {
    std::vector<std::vector<int>> vec_int{
            {0, 1, 2},
            {0, 1},
            {0},
    };
    std::cout << vec_int << std::endl;

    std::map<int, std::string> mp{
            {0, "zero"},
            {1, "one"},
            {2, "two"},
            {3, "three"},
    };
    std::cout << mp << std::endl;

    std::pair<std::vector<int>, std::string> pr{
            {0, 1, 2},
            {"zero-one-two"}
    };
    std::cout << pr << std::endl;

    std::array<int, 5> ar{1, 2, 3, 4, 5};
    std::cout << ar << std::endl;

    return 0;
}
