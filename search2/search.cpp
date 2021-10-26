#include "search.h"

SearchEngine::SearchEngine() {
    eps_ = 1e-6;
    lines_count_ = {};
    lines_list_ = {};
}

StringRef SearchEngine::GetSlice(StringRef str, size_t l, size_t r) const {
    std::string_view ans = str;
    ans.remove_prefix(l);
    ans.remove_suffix(str.size() - r - 1);
    return ans;
}

LinesList SearchEngine::GetList(const StringRef& str) const {
    LinesList list;
    size_t len = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        if (std::isalpha(str[i])) {
            ++len;
        } else {
            if (len) {
                StringRef current = str;
                current.remove_prefix(i - len);
                current.remove_suffix(str.size() - i);
                list.push_back(GetSlice(str, i - len, i - 1));
            }
            len = 0;
        }
    }
    if (len) {
        list.push_back(GetSlice(str, str.size() - len, str.size() - 1));
    }
    return list;
}

char GetSmall(char c) {
    if (c >= 'A' && c <= 'Z') {
        return static_cast<char>(c - 'A' + 'a');
    }
    return c;
}

bool SearchEngine::Cmp::operator()(const StringRef& a, const StringRef& b) const {
    for (size_t i = 0; i < std::min(a.size(), b.size()); ++i) {
        char norm_a = static_cast<char>(toupper(a[i]));
        char norm_b = static_cast<char>(toupper(b[i]));
        if (norm_a != norm_b) {
            return (norm_a < norm_b);
        }
    }
    return (a.size() < b.size());
}

bool SearchEngine::Equal(const StringRef& a, const StringRef& b) const {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        char norm_a = static_cast<char>(toupper(a[i]));
        char norm_b = static_cast<char>(toupper(b[i]));
        if (norm_a != norm_b) {
            return false;
        }
    }
    return true;
}

SearchEngine::Line::Line(const StringRef& str, long double tfdf) {
    this->str = str;
    this->tfdf = tfdf;
}

void SearchEngine::BuildIndex(StringRef text) {
    lines_count_.clear();
    lines_list_.clear();
    sum_.clear();
    size_t len = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] != '\n') {
            ++len;
        } else {
            if (len) {
                lines_count_.push_back({});
                lines_list_.push_back(GetSlice(text, i - len, i - 1));
                LinesList current = GetList(GetSlice(text, i - len, i - 1));
                for (StringRef elem : current) {
                    ++lines_count_.back()[elem];
                }
                sum_.push_back(current.size());
            }
            len = 0;
        }
    }
    if (len) {
        lines_count_.push_back({});
        lines_list_.push_back(GetSlice(text, text.size() - len, text.size() - 1));
        LinesList current = GetList(GetSlice(text, text.size() - len, text.size() - 1));
        for (StringRef elem : current) {
            ++lines_count_.back()[elem];
        }
        sum_.push_back(current.size());
    }
}

LinesList SearchEngine::Search(StringRef query, size_t results_count) const {
    std::vector<Line> lines;
    auto l = GetList(query);
    sort(l.begin(), l.end());
    LinesList new_l;
    if (!l.empty()) {
        new_l.push_back(l[0]);
        for (size_t i = 1; i < l.size(); ++i) {
            if (!SearchEngine::Equal(l[i], l[i - 1])) {
                new_l.push_back(l[i]);
            }
        }
        swap(l, new_l);
    }
    for (size_t i = 0; i < lines_list_.size(); ++i) {
        lines.emplace_back(lines_list_[i], 0);
    }
    for (const StringRef& str : l) {
        size_t counter = 0;
        size_t cnt = 0;
        for (size_t i = 0; i < lines_list_.size(); ++i) {
            auto& dict = lines_count_[i];
            size_t current = 0;
            if (dict.find(str) != dict.end()) {
                current = dict.at(str);
            }
            cnt += (sum_[i] != 0);
            counter += (current != 0);
        }
        for (size_t i = 0; i < lines_list_.size(); ++i) {
            auto& dict = lines_count_[i];
            size_t current = 0;
            if (dict.find(str) != dict.end()) {
                current = dict.at(str);
            }
            lines[i].tfdf +=
                static_cast<long double>(current) / sum_[i] * logl(static_cast<long double>(cnt) / counter);
        }
    }
    std::cerr << lines.size() << std::endl;
    std::stable_sort(lines.begin(), lines.end(), [&](const Line& a, const Line& b) { return (a.tfdf > b.tfdf); });
    while (!lines.empty() && fabsl(lines.back().tfdf) < eps_) {
        lines.pop_back();
    }
    LinesList ans;
    for (size_t i = 0; i < std::min(lines.size(), results_count); ++i) {
        ans.push_back(lines[i].str);
    }
    return ans;
}