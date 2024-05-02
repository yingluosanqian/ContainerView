//
// Created by 14199 on 2024/5/2.
//

#ifndef PLAYGROUND_CONTAINER_VIEW_H
#define PLAYGROUND_CONTAINER_VIEW_H

#include <ostream>

// check whether output function exists
template <typename T>
struct has_output_func {
    template <class U>
    static auto output(U*) -> decltype(std::declval<std::ostream&>() << std::declval<U>(), std::true_type{}) {
        return std::true_type{};
    }
    template <class U>
    static std::false_type output(...) {
        return std::false_type{};
    }
    static constexpr bool value = decltype(output<T>(nullptr))::value;
};
template <typename T>
inline constexpr bool has_output_func_v = has_output_func<T>::value;

// check whether it is a pair
template <typename T>
struct is_pair: std::false_type {};
template <typename K, typename V>
struct is_pair<std::pair<K, V>>: std::true_type {};

// function definition
template <typename Element, typename Container>
auto output_element(std::ostream& os, const Element& element, const Container&, std::true_type) ->
decltype(std::declval<typename Container::key_type>(), os);
template <typename Element, typename Container>
auto output_element(std::ostream& os, const Element& element, const Container&, ...) ->
decltype(os);

// output override
template <typename T, typename = std::enable_if_t<!has_output_func_v<T>>>
auto operator<<(std::ostream& os, const T& cont) ->
decltype(cont.begin(), cont.end(), os)
{
    using element_type = std::decay_t<decltype(*cont.begin())>;
    os << "[";
    auto it = cont.begin();
    while (it != cont.end()) {
        output_element(os, *it++, cont, is_pair<element_type>());
        if (it != cont.end()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

template <typename P1, typename P2>
std::ostream& operator << (std::ostream& os, const std::pair<P1, P2>& pr)
{
    os << "<" << pr.first << ", " << pr.second << ">";
    return os;
}

template <typename Element, typename Container>
auto output_element(std::ostream& os, const Element& element, const Container&, std::true_type) ->
decltype(std::declval<typename Container::key_type>(), os)
{
    os << element.first << ": " << element.second;
    return os;
}

template <typename Element, typename Container>
auto output_element(std::ostream& os, const Element& element, const Container&, ...) ->
decltype(os)
{
    os << element;
    return os;
}

#endif //PLAYGROUND_CONTAINER_VIEW_H
