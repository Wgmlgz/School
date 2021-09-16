#pragma once

#include <iostream>
#include <exception>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <cmath>
#include <cstdlib>

template<class F, class S>
std::ostream& operator<<(std::ostream& out, const std::pair<F, S>& pair);

template<class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& data);

template<class T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& data);

template<class K, class V>
std::ostream& operator<<(std::ostream& out, const std::map<K, V>& data);

template<class Iterator>
void PrintSequence(std::ostream& out, Iterator first, Iterator last) {
    if (first == last)
        return;
    out << *first;
    for (auto it = ++first; it != last; ++it)
        out << ", " << *it;
}

template<class F, class S>
std::ostream& operator<<(std::ostream& out, const std::pair<F, S>& pair) {
    out << "(" << pair.first << ", " << pair.second << ")";
    return out;
}

template<class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& data) {
    out << "{";
    PrintSequence(out, data.begin(), data.end());
    out << "}";
    return out;
}

template<class T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& data) {
    out << "{";
    PrintSequence(out, data.begin(), data.end());
    out << "}";
    return out;
}

template<class K, class V>
std::ostream& operator<<(std::ostream& out, const std::map<K, V>& data) {
    out << "{";
    PrintSequence(out, data.begin(), data.end());
    out << "}";
    return out;
}

#define ASSERT_EQ(expected, actual) do { \
    auto __expected = expected; \
    auto __actual = actual; \
    if (!(__expected == __actual)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl; \
        std::cerr << "\texpected: " << __expected << " (= " << #expected << ")" << std::endl; \
        std::cerr << "\tgot: " << __actual << " (= " << #actual << ")" << std::endl; \
        std::cerr << "=========== FAIL ===========\n"; \
        throw std::runtime_error("Check failed"); \
    } \
} while (false)

#define ASSERT_FLOAT_EQ(expected, actual, eps) do { \
    auto __expected = expected; \
    auto __actual = actual; \
    if (fabs(expected - actual) >= eps) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl; \
        std::cerr << "\texpected: " << __expected << " (= " << #expected << ")" << std::endl; \
        std::cerr << "\tgot: " << __actual << " (= " << #actual << ")" << std::endl; \
        std::cerr << "=========== FAIL ===========\n"; \
        throw std::runtime_error("Check failed"); \
    } \
} while (false)

#define ASSERT_TRUE(actual) do { \
    auto __actual = actual; \
    if (!(__actual == true)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl; \
        std::cerr << "\texpected: true" << std::endl; \
        std::cerr << "\tgot: " << __actual << " (= " << #actual << ")" << std::endl; \
        std::cerr << "=========== FAIL ===========\n"; \
        throw std::runtime_error("Check failed"); \
    } \
} while (false)


#define RUN_TEST(test) do { \
    std::cerr << "=========== TEST " << #test << " ===========\n"; \
    try { \
        test(); \
    } catch (...) { \
        std::terminate(); \
    } \
    std::cerr << "=========== OK! ===========\n\n"; \
} while (false)

void StartTesting() {
    std::cerr << "========================  TESTING  ========================\n\n";
}