#pragma once

#include <exception>
#include <iostream>
#include <fstream>
#include <string>

template <typename ...TArgs>
class Tuple;

class CsvParserError : public std::exception {
};

void ChangeElem(std::string &a, int& b) {
    throw CsvParserError();
}

void ChangeElem(int& a, std::string& b) {
    throw CsvParserError();
}

template<typename T>
void ChangeElem(T& a, T& b) {
    a = b;
}

template<typename THead, typename ...TTail>
class Tuple<THead, TTail...> : public Tuple<TTail...> {
    using TBase = Tuple<TTail...>;
    public:
        Tuple() : TBase(), value_(THead()) {
        }
        Tuple(THead value, TTail... tail) : TBase(tail...), value_(value) {
        }
        void Print(std::ostream& out, bool is_first) {
            if (!is_first) {
                out << ", ";
            }
            out << value_;
            TBase::Print(out, 0);
        }
        size_t len() {
            return 1 + TBase::len();
        }
        template <typename TElem>
        void Change(size_t ind, TElem& x) {
            if (ind == 0) {
                ChangeElem(value_, x);
            }
            TBase::Change(ind - 1, x);
        }
        THead value_;
};

template<>
class Tuple<> {
protected:
    Tuple() {};
    void Print(std::ostream& out, bool is_first) {
    }
    size_t len() {
        return 0;
    }
    int at(size_t ind) {
        throw std::exception();
    }
    template <typename TElem>
    void Change(size_t ind, TElem& x) {
    }
};

template<typename ...TArgs>
std::ostream& operator<<(std::ostream& out, Tuple<TArgs...> a) {
    a.Print(out, 1);
    return out;
}

template <typename... TArgs>
class CsvParser{
public:
    Tuple<TArgs...> current;
    std::string buffer;
    std::istream& in;
    char sep;
    size_t skip;
    CsvParser(std::istream& in, char sep = ',', size_t skip = 0) : in(in), sep(sep), skip(skip) {
    }
    struct Iterator {
        CsvParser& parent;
        bool f = 0;
        size_t cnt;
        Iterator(CsvParser& parent, bool f, size_t cnt) : parent(parent), f(f), cnt(cnt) {
        }
        bool operator!=(const Iterator& other) const {
            return f != other.f;
        }
        void operator++() {
            for (size_t i = 0; i < cnt + 1; ++i) {
                if (!std::getline(parent.in, parent.buffer)) {
                    if (f) {
                        throw CsvParserError();
                    }
                    f = 1;
                }
            }
            parent.current = Tuple<TArgs...>();
            cnt = 0;
        }
        Tuple<TArgs...>& operator*() {
            auto& buf = parent.buffer;
            auto& cur = parent.current;
            char separate = parent.sep;
            std::string s;
            size_t ln = cur.len();
            size_t ind = 0;
            bool is_digit = false;
            bool in_str = false;
            bool in_str_title = false;
            buf += separate;
            for (size_t i = 0; i < buf.size(); ++i) {
                if (!in_str && !in_str_title && buf[i] == ' ') {
                    continue;
                }
                if (s.empty() && std::isdigit(buf[i])) {
                    s += buf[i];
                    is_digit = true;
                    continue;
                }
                if (!in_str && buf[i] == '\"') {
                    if (!s.empty()) {
                        throw CsvParserError();
                    }
                    in_str = true;
                    continue;
                }
                if (buf[i] == separate && !in_str) {
                    in_str = false;
                    ++ind;
                    if (ind > ln) {
                        throw CsvParserError();
                    }
                    if (s.empty()) {
                        continue;
                    }
                    if (is_digit) {
                        int x = std::stoi(s);
                        cur.Change(ind - 1, x);
                    }
                    else {
                        cur.Change(ind - 1, s);
                    }
                    s.clear();
                    is_digit = false;
                    in_str_title = false;
                    continue;
                }
                if (buf[i] == '\"') {
                    if (i + 1 == buf.size() || buf[i + 1] != '\"') {
                        in_str = 0;
                        continue;
                    }
                    s += buf[i];
                    ++i;
                    continue;
                }
                if (is_digit) {
                    if (!std::isdigit(buf[i])) {
                        throw CsvParserError();
                    }
                    s += buf[i];
                    continue;
                }
                if (s.empty()) {
                    in_str_title = true;
                }
                s += buf[i];
            }
            return cur;
        }
    };
    Iterator begin() {
        auto it = Iterator(*this, false, skip);
        ++it;
        return it;
    }
    Iterator end() {
        return Iterator(*this, true, 0);
    }
};