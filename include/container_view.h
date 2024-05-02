//
// Created by 14199 on 2024/5/2.
//

#ifndef CONTAINER_VIEW_H
#define CONTAINER_VIEW_H

#include <type_traits>
#include <ostream>
#include <queue>
#include <stack>

template <typename T>
struct has_output_func {
    template <class U>
    static auto output(U*) -> decltype(std::declval<std::ostream&>() << std::declval<U>(), std::true_type{});
    template <class>
    static std::false_type output(...);
    static constexpr bool value = decltype(output<T>(nullptr))::value;
};
template <typename T>
inline constexpr bool has_output_func_v = has_output_func<T>::value;

template <typename T>
concept has_iter = requires(const T& t) {
    t.begin(); t.end();
};

template <typename T>
concept has_key_type = requires(const T& t) {
    typename T::key_type;
};

template <typename T>
concept is_pair = requires(const T& t) {
    t.first;
    t.second;
};

template <typename T>
struct Writer;

template <typename T, typename = std::enable_if_t<!has_output_func_v<T>>>
std::ostream& operator<<(std::ostream& os, const T& t) {
    return Writer<T>::write(os, t);
}

template<typename T>
struct IterWriter {
    static std::ostream& write(std::ostream& os, const T& t) {
        os << "[";
        auto it = t.begin();
        while (it != t.end()) {
            os << *it++;
            if (it != t.end()) os << ", ";
        }
        return os << "]";
    }
};

template<typename T>
struct IterKVWriter {
    static std::ostream& write(std::ostream& os, const T& t) {
        os << "{";
        auto it = t.begin();
        while (it != t.end()) {
            os << it->first << ": " << it->second;
            if (++it != t.end()) os << ", ";
        }
        return os << "}";
    }
};

struct AnyType {
    template<typename T>
    operator T();
};

template<typename T>
consteval size_t count_element(auto&& ...args) {
    if constexpr (!requires { T{args...}; }) {
        return sizeof...(args) - 1;
    } else {
        return count_element<T>(args..., AnyType());
    }
}

template<typename T>
struct Writer {
    static std::ostream& write_object(std::ostream& os, const T& t) {
        static constexpr auto num = count_element<T>();
        if constexpr (num == 1) {
            const auto& [t1] = t;
            return os << "(" << t1 << ")";
        } else if constexpr (num == 2) {
            const auto& [t1, t2] = t;
            return os << "(" << t1 << ", " << t2 << ")";
        } else if constexpr (num == 3) {
            const auto& [t1, t2, t3] = t;
            return os << "(" << t1 << ", " << t2 << ", " << t3 << ")";
        } else if constexpr (num == 4) {
            const auto& [t1, t2, t3, t4] = t;
            return os << "(" << t1 << ", " << t2 << ", " << t3 << ", " << t4 << ")";
        } else if constexpr (num == 5) {
            const auto& [t1, t2, t3, t4, t5] = t;
            return os << "(" << t1 << ", " << t2 << ", " << t3 << ", " << t4 << ", " << t5 << ")";
        } else if constexpr (num == 6) {
            const auto& [t1, t2, t3, t4, t5, t6] = t;
            return os << "(" << t1 << ", " << t2 << ", " << t3 << ", " << t4 << ", " << t5 << ", " << t6 << ")";
        } else if constexpr (num == 7) {
            const auto& [t1, t2, t3, t4, t5, t6, t7] = t;
            return os << "(" << t1 << ", " << t2 << ", " << t3 << ", " << t4 << ", " << t5 << ", " << t6 << ", " << t7 << ")";
        } else {
            return os << "Not implemented.";
        }
    }

    static std::ostream& write(std::ostream& os, const T& t) {
        if constexpr (has_iter<T>) {
            if constexpr (has_key_type<T> && is_pair<std::decay_t<decltype(*t.begin())>>) {
                return IterKVWriter<T>::write(os, t);
            } else {
                return IterWriter<T>::write(os, t);
            }
        } else {
            return write_object(os, t);
        }
    }
};

template<typename K, typename V>
struct Writer<std::pair<K, V>> {
    static std::ostream& write(std::ostream& os, const std::pair<K, V>& t) {
        return os << "(" << t.first << ", " << t.second << ")";
    }
};

template<typename... Args>
struct Writer<std::tuple<Args...>> {
    template<typename T, size_t... index>
    static std::ostream& write_tuple(std::ostream& os, const T& t, const std::index_sequence<index...>) {
        return os << "(", ((os << (index == 0 ? "" : ", ") << std::get<index>(t)), ...) << ")";
    }
    static std::ostream& write(std::ostream& os, const std::tuple<Args...>& t) {
        return write_tuple(os, t, std::index_sequence_for<Args...>());
    }
};

template<typename T>
struct Writer<std::queue<T>> {
    static std::ostream& write(std::ostream& os, const std::queue<T>& queue) {
        std::queue<T> copy_queue = queue;
        os << "[";
        while (!copy_queue.empty()) {
            auto elem = copy_queue.front();
            copy_queue.pop();
            os << elem;
            if (!copy_queue.empty()) os << ", ";
        }
        return os << "]";
    }
};

template<typename T>
struct Writer<std::stack<T>> {
    static std::ostream& write(std::ostream& os, const std::stack<T>& stack) {
        std::stack<T> copy_stack = stack;
        os << "[";
        while (!copy_stack.empty()) {
            auto elem = copy_stack.top();
            copy_stack.pop();
            os << elem;
            if (!copy_stack.empty()) os << ", ";
        }
        return os << "]";
    }
};

template<typename T, typename Container, typename Compare>
struct Writer<std::priority_queue<T, Container, Compare>> {
    static std::ostream& write(std::ostream& os, const std::priority_queue<T, Container, Compare>& p_queue) {
        std::priority_queue<T, Container, Compare> copy_p_queue = p_queue;
        os << "[";
        while (!copy_p_queue.empty()) {
            os << copy_p_queue.top();
            copy_p_queue.pop();
            if (!copy_p_queue.empty()) os << ", ";
        }
        return os << "]";
    }
};

#endif //CONTAINER_VIEW_H
