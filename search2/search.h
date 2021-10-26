#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <cctype>
#include <cmath>
#include <string_view>
#include <map>

using LinesList = std::vector<std::string_view>;
using StringRef = std::string_view;

class SearchEngine {
public:
    void BuildIndex(StringRef text);
    struct Cmp {
        bool operator()(const StringRef& a, const StringRef& b) const;
    };
    LinesList Search(StringRef query, size_t results_count) const;
    SearchEngine();
    char GetSmall(char c) const;

private:
    long double eps_;
    LinesList GetList(const StringRef& str) const;
    bool Equal(const StringRef& a, const StringRef& b) const;
    StringRef GetSlice(StringRef str, size_t l, size_t r) const;
    struct Line {
        StringRef str;
        long double tfdf;
        Line(const StringRef& str, long double tfdf);
    };
    std::vector<std::map<StringRef, size_t, Cmp>> lines_count_;
    std::vector<size_t> sum_;
    LinesList lines_list_;
};
