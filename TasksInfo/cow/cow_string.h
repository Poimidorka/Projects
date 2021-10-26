#pragma once

#include <memory>
#include <string>
#include <iostream>

using StrIterator = std::string::iterator;

class CowString{
public:
    std::shared_ptr<std::string> own;
    std::shared_ptr<size_t> cnt;
    CowString(const std::string_view& a);
    CowString(const CowString& a);
    void operator=(const CowString& a);
    struct FakeChar{
        FakeChar(size_t a, CowString& parent);
        StrIterator ptr;
        CowString& parent;
        operator char() const;
        void operator=(char c);
    };
    struct Iterator{
        Iterator(CowString& parent, size_t ind);
        CowString& parent;
        size_t ind;
        void operator++();
        bool operator!=(const Iterator& other);
        FakeChar operator*();
    };
    struct ConstIterator{
        ConstIterator(const CowString& parent, size_t ind);
        const CowString& parent;
        size_t ind;
        void operator++();
        bool operator!=(const ConstIterator& other);
        char operator*();
    };
    size_t size() const;
    char operator[](size_t ind) const;
    char At(size_t ind) const;
    FakeChar operator[](size_t ind);
    const char* GetData() const;
    operator std::string_view() const;
    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator begin();
    Iterator end();
    template<typename T2>
    bool operator==(const T2& c) const {
        std::string_view b(c);
        auto a_it = this->begin();
        if (this->size() != b.size()) {
            return false;
        }
        auto b_it = b.begin();
        while (a_it != this->end()) {
            if (*a_it != *b_it) {
                return false;
            }
            ++a_it;
            ++b_it;
        }
        return true;
    }
};

template<typename T1>
bool operator==(const T1& d, const CowString &b) {
    std::string_view a = std::string_view(d);
    auto a_it = a.begin();
    if (a.size() != b.size()) {
        return false;
    }
    auto b_it = b.begin();
    while (a_it != a.end()) {
        if (*a_it != *b_it) {
            return false;
        }
        ++a_it;
        ++b_it;
    }
    return true;
}

template<typename T1>
bool operator!=(const T1 &a, const CowString &b) {
    return !(a == b);
}

template<typename T1>
bool operator!=(const CowString &a, const T1 &b) {
    return !(a == b);
}

template <typename T1>
void operator+=(CowString &str, const T1& c) {
    std::string_view b(c);
    if (*str.cnt != 1) {
        std::string* new_string = new std::string(*(str.own));
        str.own = std::shared_ptr<std::string>(new_string);
        --(*(str.cnt));
        str.cnt = std::shared_ptr<size_t>(new size_t(1));
    }
    auto it = b.begin();
    while (it != b.end()) {
        *(str.own) += *it;
        ++it;
    }
}